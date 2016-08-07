#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif


#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDerivativeImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkImage.h"
#include <vnl/vnl_math.h>
#include "itkMultiplyImageFilter.h"
#include <itkImageRegionIterator.h>
#include <itkImageRegionIterator.h>
#include <itkDivideByConstantImageFilter.h>
#include <itkWarpVectorImageFilter.h>
#include <tclap/CmdLine.h>

/*
 * The program implements the iterative computation of the exponential of a stationary velocity field (SVF).
 * Given the one-parameter subgroups property exp(2v)=exp(v) o exp(v), 
 * the exponential is iteratively computed through the following scheme:
 *
 * Step1. Find n such that v0=inputSVF/2^n "small" (scaling step) 
 * Step2. Iterate n times Exp(vi)=Exp(vi-1) o Exp(vi-1) (squaring step)
 *
 * The program requires the input SVF image path. 
 * It is possible to provide a Mask for the computation of scaling step (useful to control the computation on the boundaries),
 * and to provide a scaling factor for the input SVF.
 * The program allows also to compute the exponential by using a forward Euler scheme (-e option):
 *
 * Step1. Find n such that v0=inputSVF/n "small" (scaling step) 
 * Step2. Iterate n times Exp(vi)=Exp(vi-1) o Exp(v0) (Euler integtration)
 *
 * In this case the computation time is higher, but the accuracy is improved, especially in case of large displacements.
 */

/**
 * Structure containing the input parameters.
 */
struct Param{
    std::string  SVFImage;
    std::string  Mask;
    std::string  OutputImage;
    float ScalingFactor;
    bool Scheme;
};


/**
 * Parses the command line arguments and deduces the corresponding Param structure.
 * @param  argc   number of arguments
 * @param  argv   array containing the arguments
 * @param  param  structure of parameters
 */
void parseParameters(int argc, char** argv, struct Param & param)
{

    // Program description
    std::string description = "\b\b\bDESCRIPTION\n";
    description += "Stationary Velocity Field Exponential Computation";
    description += "\nAuthor : Marco Lorenzi";

    try {

        // Define the command line parser
        TCLAP::CmdLine cmd( description, ' ', "1.0", true);

        TCLAP::ValueArg<std::string>  arg_SVFImage( "i", "input-svf", "Path to the input stationary velocity field", true, "", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_OutputImage( "o", "output-svf", "Path of the output LogJacobian map (default LogJacobian.mha).", false, "LogJacobian.mha", "string", cmd );
        TCLAP::ValueArg<std::string>  arg_Mask( "m", "mask", "Path to the mask (default whole image)", false, "null", "string", cmd );
        TCLAP::ValueArg<double>  arg_ScalingFactor( "s", "scaling-factor", "Scaling factor for the input velocity field (default 1)", false, 1.0, "double", cmd );
 	TCLAP::ValueArg<bool>  arg_Scheme( "e", "computational-scheme", "Computational scheme: 0 (default) Scaling and Squarings (faster), 1 Forward Euler (more accurate)", false, 0, "bool", cmd );

        // Parse the command line
        cmd.parse( argc, argv );

        // Set the parameters
        param.SVFImage                     = arg_SVFImage.getValue();
        param.OutputImage                  = arg_OutputImage.getValue();
        param.Mask                         = arg_Mask.getValue();
        param.ScalingFactor                = arg_ScalingFactor.getValue();
	param.Scheme			   = arg_Scheme.getValue();
    }
    catch (TCLAP::ArgException &e)
    {
        std::cerr << "Error: " << e.error() << " for argument " << e.argId() << std::endl;
        throw std::runtime_error("Unable to parse the command line arguments.");
    }
}


/**
  * Prints parameters.
  * @param  SVFImage                        path to the input SVF
  * @param  OutputImage      		    path to the output log jacobian map
  * @param  Mask                 	    path to the mask
  * @param  Scaling Factor                  ScalingFactor
  * @param  Computational Scheme	    Scheme
  */
void PrintParameters( std::string SVFImage,
                      std::string OutputImage,
                      std::string Mask,
                      double      ScalingFactor,
		      bool  Scheme)
{

    // Print I/O parameters
    std::cout << std::endl;
    std::cout << "I/O PARAMETERS"                  << std::endl;
    std::cout << "  Input SVF image path           : " << SVFImage      << std::endl;
    std::cout << "  Output image path              : " << OutputImage     << std::endl;
    std::cout << "  Mask image path                : " << Mask     << std::endl;
    std::cout << "  Scaling Factor                 : " << ScalingFactor << std::endl;
     std::cout << "  Computational Scheme          : " << Scheme << std::endl;

    std::cout << std::endl;
}



int main( int argc, char ** argv )
{
  
  //Parsing initial parameters
  struct Param param;
  parseParameters( argc, argv, param);
   
  //Definition of the type:  
  typedef itk::Vector<float,2>  VectorPixelType;
  typedef itk::Image<VectorPixelType,2> VectorImageType;	
  typedef itk::Image<float,2> ImageType;


  //Definition of Reader and Writer Classes:
  typedef itk::ImageFileReader< VectorImageType >  VectorReaderType;
  typedef itk::ImageFileWriter< VectorImageType >  VectorWriterType;
  typedef itk::ImageFileReader< ImageType >  ScalarReaderType;


  float mult=param.ScalingFactor;
  //Instantiation of a reader for the SVF
  VectorReaderType::Pointer reader1 = VectorReaderType::New();
  try
  {
   reader1->SetFileName(param.SVFImage);
   reader1->Update();
  }
  catch 
   (TCLAP::ArgException &e)
  {
   std::cerr << "Error: " << e.what() << std::endl;
   return EXIT_FAILURE;
  }



typedef itk::ImageRegionIterator<VectorImageType> IteratorType;  

  VectorImageType::Pointer Out1=VectorImageType::New();
  Out1->SetRegions(reader1->GetOutput()->GetLargestPossibleRegion() );
  Out1->SetSpacing( reader1->GetOutput()->GetSpacing() );
  Out1->SetOrigin( reader1->GetOutput()->GetOrigin() );
  Out1->SetDirection( reader1->GetOutput()->GetDirection() );
  Out1->Allocate();
  

// The input SVF is scaled according to the input scaling factor
  
  IteratorType out1(Out1,reader1->GetOutput()->GetLargestPossibleRegion());
 
  typedef itk::Index<2> IndexType;
  itk::Vector<float,2> Vect;  
 // std::cout<<"******* "<< mult<<" ********"<<std::endl;
  for (out1.GoToBegin();!out1.IsAtEnd();++out1 ) 
  { Vect.Fill(0);
    const IndexType index=out1.GetIndex();
    Vect= mult*(reader1->GetOutput()->GetPixel( index ));
    out1.Set( Vect);
  }

  VectorWriterType::Pointer writer = VectorWriterType::New();
  ScalarReaderType::Pointer readerMask = ScalarReaderType::New();

  //Max norm and scaling factor computation
  double minpixelspacing = Out1->GetSpacing()[0];
    for (unsigned int i = 0; i<2; ++i)
      {
      if ( Out1->GetSpacing()[i] < minpixelspacing/2 )
        {
        minpixelspacing = Out1->GetSpacing()[i];
        }
      }

   
    IteratorType InputIt (Out1, Out1->GetRequestedRegion());

 float norm2,maxnorm2=0;

 if (param.Mask!="null")
   try
    {
     readerMask->SetFileName(param.Mask);
     readerMask->Update();	
    }
   catch(TCLAP::ArgException &e)
    {
     std::cerr << "Error: " << e.what() << std::endl;
     return EXIT_FAILURE;
    }

 /* 
   *   Evaluate the maximum norm in the region of interest
   */
  if (param.Mask!="null")
   {
    for( InputIt.GoToBegin(); !InputIt.IsAtEnd(); ++InputIt )
      {
       if (readerMask->GetOutput()->GetPixel(InputIt.GetIndex())>0)
        {
         norm2 = InputIt.Get().GetSquaredNorm();
         if (maxnorm2<norm2) 
           maxnorm2=norm2;
         }
       }
     }
    else 
    {
     for( InputIt.GoToBegin(); !InputIt.IsAtEnd(); ++InputIt )
     {
       norm2 = InputIt.Get().GetSquaredNorm();
       if (maxnorm2<norm2) 
       maxnorm2=norm2;
     }
    }
   
  float numiter;
  int divider;
   if (param.Scheme==1)
     {
      numiter = ceil(2*pow(maxnorm2,0.5)/minpixelspacing+0.5);
      divider = numiter;
     }
   else 
     {
     numiter = (2.0 +   0.5 * vcl_log(maxnorm2)/vnl_math::ln2);
     divider = 1<<static_cast<unsigned int>(numiter);
     }


if (numiter<1) 
{numiter=1;
divider=2;}



  //The SVF is scaled     
  typedef itk::DivideByConstantImageFilter<VectorImageType,float,VectorImageType> DividerType;
  DividerType::Pointer Divider=DividerType::New();
  Divider->SetInput(Out1);
  Divider->SetConstant(divider);
 
  Divider->Update();

  typedef itk::WarpVectorImageFilter<VectorImageType,VectorImageType,VectorImageType> WarperType;
    typedef itk::AddImageFilter<VectorImageType,VectorImageType,VectorImageType> AdderType;

  AdderType::Pointer Adder=AdderType::New();  
  WarperType::Pointer Warper=WarperType::New();
  Warper->SetOutputOrigin(Out1->GetOrigin());
  Warper->SetOutputSpacing(Out1->GetSpacing());
  Warper->SetOutputDirection(Out1->GetDirection());
  
  
   VectorImageType::Pointer Vect1=VectorImageType::New();
   VectorImageType::Pointer warpedIm=VectorImageType::New();

    Vect1=Divider->GetOutput();

std::string str;

  //Exponentiation of the scaled SVF
  //
  //Forward Euler scheme.
  if (param.Scheme==1)
   for( int i=0; i<abs(numiter); i++ )
    {
    if (i==0)
    Warper->SetInput(Vect1);
    else
    Warper->SetInput(Adder->GetOutput());

    Warper->SetDeformationField(Vect1);

    Warper->GetOutput()->SetRequestedRegion(
       Vect1->GetRequestedRegion() );

    Warper->Update();

    warpedIm = Warper->GetOutput();
  
    if (i==0)
    Adder->SetInput1(Vect1);

    Adder->SetInput2(warpedIm);

    Adder->GetOutput()->SetRequestedRegion(
      Vect1->GetRequestedRegion() );

    Adder->Update();
    
    }
  else 
  //Scaling and Squarings scheme
   for( int i=0; i<abs(numiter); i++ )
   {
    Warper->SetInput(Vect1);
    
    Warper->SetDeformationField(Vect1);

    Warper->GetOutput()->SetRequestedRegion(
       Vect1->GetRequestedRegion() );

    Warper->Update();

    warpedIm = Warper->GetOutput();
  
    Adder->SetInput1(Vect1);

    Adder->SetInput2(warpedIm);

    Adder->GetOutput()->SetRequestedRegion(Vect1->GetRequestedRegion() );
    Adder->Update();
    Vect1=Adder->GetOutput(); 
    Vect1->DisconnectPipeline();
   }

  //Writing the output
  writer->SetFileName( param.OutputImage);
  writer->SetInput( Adder->GetOutput());
  
  try 
    { 
    writer->Update(); 
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;}

 
  return EXIT_SUCCESS;





}

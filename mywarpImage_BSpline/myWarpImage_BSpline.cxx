#include "itkImageFileReader.h"
#include "itkImage.h"
#include <itkWarpImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "itkResampleImageFilter.h"
#include <itkNearestNeighborInterpolateImageFunction.h>
#include "itkBSplineInterpolateImageFunction.h"

int main(int argc, char** argv){
  

  
   typedef itk::Vector<float,2>  VectorPixelType;
   typedef itk::Image<VectorPixelType,2> VectorImageType;	
   typedef itk::Image<float,2> ImageType;

//Definition of Reader and Writer Classes:
  typedef itk::ImageFileReader< VectorImageType >  VectorReaderType;
  typedef itk::ImageFileReader< ImageType >  ImageReaderType;
  typedef itk::ImageFileWriter< ImageType >  ImageWriterType;
  
  ImageReaderType::Pointer ImageReader= ImageReaderType::New();
  VectorReaderType::Pointer VectorReader= VectorReaderType::New();
  

  
  ImageReader->SetFileName(argv[1]);
  VectorReader->SetFileName(argv[2]);
  
  ImageReader->Update();
  VectorReader->Update();
  
 typedef itk::WarpImageFilter< ImageType,ImageType,VectorImageType >  WarperType;

typedef itk::BSplineInterpolateImageFunction<ImageType, double, double> T_Interpolator;

T_Interpolator::Pointer _pInterpolator = T_Interpolator::New();
_pInterpolator->SetSplineOrder(3);


/////ooooold ne pas toucher
//typedef WarperType::InterpolatorType WInterpType;
//typedef itk::NearestNeighborInterpolateImageFunction <ImageType,double> InterpolatorType;
 // InterpolatorType::Pointer Interpolator=InterpolatorType::New();

    
  WarperType::Pointer warper=WarperType::New();
  
  warper->SetDeformationField(VectorReader->GetOutput());
  warper->SetOutputSpacing( ImageReader->GetOutput()->GetSpacing() );
  warper->SetOutputOrigin( ImageReader->GetOutput()->GetOrigin() );
  warper->SetOutputDirection( ImageReader->GetOutput()->GetDirection() );
  warper->SetInput(ImageReader->GetOutput());
 warper->SetInterpolator(_pInterpolator); 
  warper->Update();

  ImageWriterType::Pointer ImageWriter= ImageWriterType::New();
  ImageWriter->SetInput(warper->GetOutput());
  ImageWriter->SetFileName(argv[3]);
  ImageWriter->Update();
  
  return 0;
  
}
  
  
  
  
  

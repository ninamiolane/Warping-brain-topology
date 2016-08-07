#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include"itkImageIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"


using namespace std;

//main

int main (int argc, char ** argv)
{

typedef float PixelType;
const unsigned int Dimension=2;


typedef itk::Vector<PixelType, Dimension> VectorPixelType;
typedef itk::Image<VectorPixelType, Dimension> ImageType;

//ImageType::Pointer PivotImage=ImageType::New();
//ImagePointer* Array_reader = new ImagePointer[argc-1];

typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;

typedef itk::ImageRegionIterator< ImageType > VectorIteratorType;

// ligne sous a enlever
ReaderType::Pointer reader1=ReaderType::New();
WriterType::Pointer writer=WriterType::New();

reader1->SetFileName(argv[1]);
reader1->Update();

ImageType::Pointer input_image1 = reader1->GetOutput();

//definition nouvelle image
ImageType::Pointer output_image = ImageType::New();
output_image->SetRegions(input_image1->GetLargestPossibleRegion());
output_image->SetSpacing(input_image1->GetSpacing());
output_image->SetOrigin(input_image1->GetOrigin());
output_image->SetDirection(input_image1->GetDirection());

output_image->Allocate();

VectorIteratorType out1(output_image, input_image1->GetLargestPossibleRegion());

typedef itk::Index<Dimension> IndexType;

itk::Vector<float,Dimension> Vectnull;



for (out1.GoToBegin(); !out1.IsAtEnd(); ++out1)
{
    Vectnull.Fill(0);
    out1.Set(Vectnull);
}

//le champ de vecteur final existe maintenant et vaut 0 partout

//

//definition dun reader par image pour toutes les images
for (int i=1;i<argc-1;++i)
{
    ReaderType::Pointer reader=ReaderType::New();
    ImageType::Pointer input_image = reader->GetOutput();
    std::cout << "Reading " << argv[i] << "..." << std::endl;
    reader->SetFileName(argv[i]);
    reader->Update();


    for (out1.GoToBegin(); !out1.IsAtEnd(); ++out1)
    {
        const IndexType index=out1.GetIndex();

        if (i==(argc-2))
            out1.Set(((input_image->GetPixel(index))+(out1.Get()))/(argc-2));
        else
            out1.Set((input_image->GetPixel(index))+(out1.Get()));
    }


}


//////////////////
writer->SetInput(output_image);
writer->SetFileName(argv[argc-1]);

try
{
writer->Update();
}
catch( itk::ExceptionObject & err )
  {
  std::cerr << "ExceptionObject caught !" << std::endl;
  std::cerr << err << std::endl;
  return EXIT_FAILURE;
  }

return EXIT_SUCCESS;

}









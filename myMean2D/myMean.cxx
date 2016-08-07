#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkImageIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

using namespace std;

//main

int main (int argc, char ** argv)
{

typedef float PixelType;
const unsigned int Dimension=2;

typedef itk::Image<PixelType, Dimension> ImageType;

typedef ImageType::Pointer ImagePointer;
ImagePointer* Array_reader = new ImagePointer[argc-1];

typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;

typedef itk::ImageRegionIterator< ImageType > VectorIteratorType;

//ReaderType::Pointer reader=ReaderType::New();
WriterType::Pointer writer=WriterType::New();


//ImageType::Pointer input_image = reader->GetOutput();

//definition dun reader par image pour toutes les images
for (int i=1;i<argc-1;++i)
{
    ReaderType::Pointer reader=ReaderType::New();
    ImageType::Pointer input_image = reader->GetOutput();
    std::cout << "Reading " << argv[i] << "..." << std::endl;
    reader->SetFileName(argv[i]);
    reader->Update();
    Array_reader[i]=input_image;
}

Array_reader[1]->Update();

//definition nouvelle image
ImageType::Pointer output_image = ImageType::New();
output_image->SetRegions(Array_reader[1]->GetLargestPossibleRegion());
output_image->SetSpacing(Array_reader[1]->GetSpacing());
output_image->SetOrigin(Array_reader[1]->GetOrigin());
output_image->SetDirection(Array_reader[1]->GetDirection());

output_image->Allocate();



///////////////////////

VectorIteratorType out(output_image, Array_reader[1]->GetLargestPossibleRegion());


typedef itk::Index<Dimension> IndexType; //3 dimensional index in 3D image
float Mean;

for (out.GoToBegin(); !out.IsAtEnd(); ++out)
{
    const IndexType index=out.GetIndex();
    Mean=0;
    for(int i=1; i<argc-1; ++i)
    {
        Mean+=(Array_reader[i]->GetPixel(index));
    }
    out.Set(Mean / (argc-2));
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








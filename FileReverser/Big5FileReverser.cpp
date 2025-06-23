#include "Big5FileReverser.h"
#include "DataProvider/MmapDataProvider.h"
#include "DataProvider/StreamDataProvider.h"
#include "EncodingFixer/Big5EncodingFixer.h"
#include "FileReverser.h"

std::shared_ptr<IFileReverser> Big5FileReverser::MakeMmapReverser(const char* filepath)
{
    return std::make_shared<FileReverser>(
        std::make_shared<MmapDataProvider>(filepath),
        std::make_shared<Big5EncodingFixer>()
    );
}
std::shared_ptr<IFileReverser> Big5FileReverser::MakeStreamReverser(const char* filepath)
{
    return std::make_shared<FileReverser>(
        std::make_shared<StreamDataProvider>(filepath),
        std::make_shared<Big5EncodingFixer>()
    );  
}
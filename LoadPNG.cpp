//
// Created by monty on 06/10/16.
//
#include <memory>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "IFileLoaderDelegate.h"
#include "NativeBitmap.h"
#include "LoadPNG.h"

#include "stb_image.h"

std::shared_ptr<odb::NativeBitmap> loadPNG(const std::string filename, std::shared_ptr<Knights::IFileLoaderDelegate> fileLoader ) {

    auto buffer = fileLoader->loadBinaryFileFromPath( filename );
    int xSize;
    int ySize;
    int components;
    
#if defined(TARGET_IOS)
    stbi_convert_iphone_png_to_rgb(1);
#endif
    
    auto image = stbi_load_from_memory((const stbi_uc *) buffer.data(), buffer.size(), &xSize, &ySize, &components, 0 );
    auto rawData = new int[ xSize * ySize ];
    std::memcpy( rawData, image, xSize * ySize * 4 );
    stbi_image_free(image);

    return std::make_shared<odb::NativeBitmap>( filename, xSize, ySize, rawData );
}

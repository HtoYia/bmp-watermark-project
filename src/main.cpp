#include<iostream>
#include "bmpHandler.h"
#include "imageBMP.h"
#include "watermark.h"
int main(int argc, char* argv[]) { 
    if (argc != 4) { 
        std::cerr << "Incorrect input. Please use format " << argv[0] << " <original.bmp> <watermark.bmp> <output.bmp>\n";
        return 1;  
    }  
    const char* original_file = argv[1];
    const char* watermark_file = argv[2]; 
    const char* result_file = argv[3]; 
    ImageBMP original; 
    ImageBMP watermark; 
    
    bool check_original = HandlerBMP::Read(original_file, original); 
    if (!check_original) { 
        std::cerr << "Failed to read original file: " << original_file << "\n"; 
        return 1; 
    } 

    bool check_watermark = HandlerBMP::Read(watermark_file, watermark); 
    if (!check_watermark) { 
        std::cerr << "Failed to read watermark file " << watermark_file << "\n"; 
        return 1; 
    }

    auto original_W = original.GetWidth(), original_H = original.GetHeight(); 
    auto watermark_W = watermark.GetWidth(), watermark_H = watermark.GetHeight(); 
    if (original_W < watermark_W || original_H < watermark_H) { 
        std::cerr << "Watermark size is bigger than the original one"; 
        return 1;
    }

    ApplyWatermark(original, watermark); 
    bool check_result = HandlerBMP::Write(original, result_file, false);
    if (!check_result) { 
        std::cerr << "Failed to wright file" << result_file << "\n"; 
        return 1; 
    }
    std::cout << "Image saved to " << result_file << "\n"; 
    return 0; 
}
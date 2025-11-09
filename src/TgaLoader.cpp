#include <iostream>
#include <fstream>
#include <utility>
#include "TgaLoader.h"
#include <cstring>

constexpr int SIGNATURE_SIZE = 12;
constexpr int BITS_PER_BYTE = 8;

bool LoadTGA(const char *filename, unsigned char *&data, unsigned short &width, unsigned short &height)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Could not open file " << filename << "." << std::endl;
        return false;
    }

    // Read and validate TGA signature
    constexpr unsigned char CORRECT_SIGNATURE[] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned char signature[SIGNATURE_SIZE];

    file.read(reinterpret_cast<char*>(signature), SIGNATURE_SIZE);

    if (!file || memcmp(signature, CORRECT_SIGNATURE, SIGNATURE_SIZE) != 0)
    {
        std::cerr << filename << " is an invalid TGA file." << std::endl;
        return false;
    }

    // Read image dimensions and bits per pixel
    unsigned short bpp; // bits per pixel

    file.read(reinterpret_cast<char*>(&width), sizeof(unsigned short));
    file.read(reinterpret_cast<char*>(&height), sizeof(unsigned short));
    file.read(reinterpret_cast<char*>(&bpp), sizeof(unsigned char));  // bpp is 1 byte in TGA format
    file.ignore(1);  // Skip 1 byte (image descriptor)

    if (!file)
    {
        std::cerr << "Could not read TGA header data." << std::endl;
        return false;
    }

    bpp /= BITS_PER_BYTE;  // Convert bits per pixel to bytes per pixel

    // Allocate memory for pixel data
    unsigned int dataLength = width * height * bpp;
    data = static_cast<unsigned char*>(malloc(dataLength));
    
    if (!data)
    {
        std::cerr << "Could not allocate memory for the TGA image." << std::endl;
        return false;
    }

    // Read pixel data
    file.read(reinterpret_cast<char*>(data), dataLength);

    // Check if we read the expected amount of data
    if (file.gcount() != static_cast<std::streamsize>(dataLength))
    {
        std::cerr << "Could not read TGA pixel data. Expected " << dataLength 
                  << " bytes, got " << file.gcount() << " bytes." << std::endl;
        free(data);
        data = nullptr;
        return false;
    }

    return true;
}

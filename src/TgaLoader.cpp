#include <iostream>
#include <fstream>
#include <vector>
#include "TgaLoader.h"
#include <cstring>

constexpr int SIGNATURE_SIZE = 12;
constexpr int BITS_PER_BYTE = 8;

bool LoadTGA(const char *filename, std::vector<unsigned char> &data, unsigned short &width, unsigned short &height)
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

    file.read(reinterpret_cast<char *>(signature), SIGNATURE_SIZE);

    if (!file || memcmp(signature, CORRECT_SIGNATURE, SIGNATURE_SIZE) != 0)
    {
        std::cerr << filename << " is an invalid TGA file." << std::endl;
        return false;
    }

    // Read image dimensions and bits per pixel
    unsigned short bpp; // bits per pixel
    unsigned char imageDescriptor;

    file.read(reinterpret_cast<char *>(&width), sizeof(unsigned short));
    file.read(reinterpret_cast<char *>(&height), sizeof(unsigned short));
    file.read(reinterpret_cast<char *>(&bpp), sizeof(unsigned char));             // bpp is 1 byte in TGA format
    file.read(reinterpret_cast<char *>(&imageDescriptor), sizeof(unsigned char)); // Read image descriptor

    if (!file)
    {
        std::cerr << "Could not read TGA header data." << std::endl;
        return false;
    }

    bpp /= BITS_PER_BYTE; // Convert bits per pixel to bytes per pixel

    // Allocate memory for pixel data using vector
    unsigned int dataLength = width * height * bpp;
    data.resize(dataLength);

    // Read pixel data
    file.read(reinterpret_cast<char *>(data.data()), dataLength);

    // Check if we read the expected amount of data
    if (file.gcount() != static_cast<std::streamsize>(dataLength))
    {
        std::cerr << "Could not read TGA pixel data. Expected " << dataLength
                  << " bytes, got " << file.gcount() << " bytes." << std::endl;
        data.clear();
        return false;
    }

    // Check if image needs vertical flip based on origin
    // Bit 5 of image descriptor: 0 = Origin at top-left (need flip), 1 = Origin at bottom-left (no flip needed for OpenGL)
    bool hasTopLeftOrigin = (imageDescriptor & 0x20) == 0;

    if (hasTopLeftOrigin)
    {
        // Flip the image vertically to convert bottom-left to top-left for OpenGL
        unsigned int rowSize = width * bpp;
        std::vector<unsigned char> tempRow(rowSize);

        for (unsigned int y = 0; y < height / 2; ++y)
        {
            unsigned int topRowOffset = y * rowSize;
            unsigned int bottomRowOffset = (height - 1 - y) * rowSize;

            // Swap rows
            std::memcpy(tempRow.data(), &data[topRowOffset], rowSize);
            std::memcpy(&data[topRowOffset], &data[bottomRowOffset], rowSize);
            std::memcpy(&data[bottomRowOffset], tempRow.data(), rowSize);
        }
    }

    return true;
}

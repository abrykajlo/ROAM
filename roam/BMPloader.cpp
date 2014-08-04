#include <iostream>
#include <cstdlib>
#include <cmath> 
#include <fstream>
#include <vector>

using namespace std;

#include <inttypes.h>

typedef struct                       /**** BMP file header structure ****/
{
    unsigned short bfType;           /* Magic number for file */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} BITMAPFILEHEADER;

typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} BITMAPINFOHEADER;

uint8_t* LoadBMP(const char* location, int &w, int &h) {
    uint8_t* datBuff[2] = {0, 0}; // Header buffers

    uint8_t* pixels = 0;

    BITMAPFILEHEADER* bmpHeader = 0; // Header
    BITMAPINFOHEADER* bmpInfo   = 0; // Info 

    // The file... We open it with it's constructor
    std::ifstream file(location, std::ios::binary);
    if(!file)
    {
        std::cout << "Failure to open bitmap file.\n";

        return NULL;
    }

    // Allocate byte memory that will hold the two headers
    datBuff[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
    datBuff[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];

    file.read((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
    file.read((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

    // Construct the values from the buffers
    bmpHeader = (BITMAPFILEHEADER*) datBuff[0];
    bmpInfo   = (BITMAPINFOHEADER*) datBuff[1];

    // Check if the file is an actual BMP file
    if(bmpHeader->bfType != 0x4D42)
    {
        std::cout << "File \"" << location << "\" isn't a bitmap file\n";
        return NULL;
    }

    // First allocate pixel memory
    pixels = new uint8_t[bmpInfo->biSizeImage];

    // Go to where image data starts, then read in image data
    file.seekg(bmpHeader->bfOffBits);
    file.read((char*)pixels, bmpInfo->biSizeImage);

    // We're almost done. We have our image loaded, however it's not in the right format.
    // .bmp files store image data in the BGR format, and we have to convert it to RGB.
    // Since we have the value in bytes, this shouldn't be to hard to accomplish
    uint8_t tmpRGB = 0; // Swap buffer
    for (unsigned long i = 0; i < bmpInfo->biSizeImage; i += 3)
    {
        tmpRGB        = pixels[i];
        pixels[i]     = pixels[i + 2];
        pixels[i + 2] = tmpRGB;
    }

    // Set width and height to the values loaded from the file
    w = (int)bmpInfo->biWidth;
    h = (int)bmpInfo->biHeight;
    


    return pixels;
}

int main(int argc, char const *argv[]) {

    int w, h;
    uint8_t* img = 0;
    img = LoadBMP("test.bmp", w, h);

   	cout << (int)img[4];

    return 0;
}
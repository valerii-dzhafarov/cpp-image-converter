#include "bmp_image.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

constexpr int BYTE_PER_PIX = 3;
constexpr int ALING_4 = 4;

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return ALING_4 * ((w * BYTE_PER_PIX + BYTE_PER_PIX) / ALING_4);
}

static int GetBMPPadding(int w) {
    return GetBMPStride(w) - w*BYTE_PER_PIX;
}


BitmapFileHeader::BitmapFileHeader(int32_t w, int32_t h) {
    padding = (sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader));
    file_size = padding + GetBMPStride(w) * h;
}

BitmapInfoHeader::BitmapInfoHeader(int32_t w, int32_t h) :
        width(w), 
        height(h), 
        byte_size(GetBMPStride(w) * h) 
{}


// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    ofstream out(file, ios::binary);
    int width  = image.GetWidth();
    int height = image.GetHeight();
    
    BitmapFileHeader file_hdr(width, height);
    BitmapInfoHeader info_hdr(width, height);

    out.write(reinterpret_cast<char*>(&file_hdr), sizeof(BitmapFileHeader));
    out.write(reinterpret_cast<char*>(&info_hdr), sizeof(BitmapInfoHeader));

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            auto pix = image.GetPixel(x,y);
            out << static_cast<char>(pix.b) 
                << static_cast<char>(pix.g) 
                << static_cast<char>(pix.r);
        }
        for(int x = 0; x < GetBMPPadding(width); ++x) {
            out.put('\0');
        }
    }
    return out.good();
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    ifstream in(file, ios::binary);

    BitmapFileHeader file_hdr;
    in.read(reinterpret_cast<char*>(&file_hdr), sizeof(BitmapFileHeader));
    if (!in) {
        return {};
    }

    if (file_hdr.signature[0] != 'B' || file_hdr.signature[1] != 'M') {
        return {};
    }
    
    BitmapInfoHeader info_hdr;
    in.read(reinterpret_cast<char*>(&info_hdr), sizeof(BitmapInfoHeader));
    if (!in) {
        return {};
    }

    int height = info_hdr.height;
    int width = info_hdr.width;
    
    Image image(width, height, Color::Black());

    int stride = GetBMPStride(width);
    std::vector<byte> byte_buff (stride, 0);

    for (int y = height - 1; y >= 0; --y) {
        in.read(byte_buff.data(), stride);
        if (!in) {
            return {};
        }
        for (int x = 0; x < BYTE_PER_PIX*width; ) {
            auto& pix = image.GetPixel(x,y);
            pix.b = (byte_buff[x++]);
            pix.g = (byte_buff[x++]);
            pix.r = (byte_buff[x++]);
        }
    
    }
    return image;
}

}  // namespace img_lib
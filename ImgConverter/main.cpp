#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>
#include <bmp_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

enum class Format{
    JPEG, PPM, BMP, UNKNOWN
};

class ImageFormatInterface {
public:
    virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
    virtual img_lib::Image LoadImage(const img_lib::Path& file) const = 0;
};

class PpmFormatInterface : public ImageFormatInterface{
public:
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
        return img_lib::SavePPM(file, image);
    };

    img_lib::Image LoadImage(const img_lib::Path& file) const override {
        return img_lib::LoadPPM(file);
    }
};

class JpegFormatInterface : public ImageFormatInterface{
public:
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
        return img_lib::SaveJPEG(file, image);
    };

    img_lib::Image LoadImage(const img_lib::Path& file) const override {
        return img_lib::LoadJPEG(file);
    }
};


class BmpFormatInterface : public ImageFormatInterface{
public:
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
        return img_lib::SaveBMP(file, image);
    };

    img_lib::Image LoadImage(const img_lib::Path& file) const override {
        return img_lib::LoadBMP(file);
    }
};


Format GetFormatByExtension(const img_lib::Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return Format::PPM;
    }

    if (ext == ".bmp"sv) {
        return Format::BMP;
    }

    return Format::UNKNOWN;
}

const ImageFormatInterface* GetFormatInterface(const img_lib::Path& path) {
    static const JpegFormatInterface jpegInterface;
    static const PpmFormatInterface  ppmInterface;
    static const BmpFormatInterface  bmpInterface;

    switch (GetFormatByExtension(path))
    {
    case  Format::JPEG:
        return &jpegInterface;
    case  Format::PPM:
        return &ppmInterface;
    case  Format::BMP:
        return &bmpInterface;
    
    default:
        return nullptr;
    }
}
/*
Добавьте в main следующие коды возврата и сообщения об ошибках:
Неизвестный формат входного файла — Unknown format of the input file. Код возврата 2.
Неизвестный формат выходного файла — Unknown format of the output file. Код возврата 3.
*/

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];

    auto* in_intf  =  GetFormatInterface(in_path);
    if (!in_intf) {
        cerr << "Unknown format of the input file" << endl;
        return 2;
    }

    auto* out_intf =  GetFormatInterface(out_path);
    if (!out_intf) {
        cerr << "Unknown format of the output file" << endl;
        return 3;
    }

    img_lib::Image image = in_intf->LoadImage(in_path);
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }

    if (!out_intf->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }

    cout << "Successfully converted"sv << endl;
}
#pragma once

#include "img_lib.h"
#include "pack_defines.h"

#include <filesystem>

namespace img_lib {

using Path = std::filesystem::path;

PACKED_STRUCT_BEGIN BitmapFileHeader {
    //Подпись — 2 байта. Символы BM.
    char signature [2] = {'B','M'};
    //Суммарный размер заголовка и данных — 4 байта, беззнаковое целое. 
    //  Размер данных определяется как отступ, умноженный на высоту изображения.
    uint32_t file_size ;
    //Зарезервированное пространство — 4 байта, заполненные нулями.
    uint32_t reserve = 0;
    //Отступ данных от начала файла — 4 байта, беззнаковое целое. Он равен размеру двух частей заголовка.
    uint32_t padding;

    BitmapFileHeader() = default;
    BitmapFileHeader(int32_t w, int32_t h);

}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    //Размер заголовка — 4 байта, беззнаковое целое. Учитывается только размер второй части заголовка.
    uint32_t hdr_size = sizeof(BitmapInfoHeader);
    //Ширина изображения в пикселях — 4 байта, знаковое целое.
    int32_t width;
    //Высота изображения в пикселях — 4 байта, знаковое целое.
    int32_t height;
    //Количество плоскостей — 2 байта, беззнаковое целое. В нашем случае всегда 1 — одна RGB плоскость.
    uint16_t surface_num = 1;
    //Количество бит на пиксель — 2 байта, беззнаковое целое. В нашем случае всегда 24.
    uint16_t bit_per_pix = 24;
    //Тип сжатия — 4 байта, беззнаковое целое. В нашем случае всегда 0 — отсутствие сжатия.
    uint32_t copression = 0;
    //Количество байт в данных — 4 байта, беззнаковое целое. Произведение отступа на высоту.
    uint32_t byte_size = 0;
    //Горизонтальное разрешение, пикселей на метр — 4 байта, знаковое целое. Нужно записать 11811, что примерно соответствует 300 DPI.
    int32_t hor_pix_per_m = 11811;
    //Вертикальное разрешение, пикселей на метр — 4 байта, знаковое целое. Нужно записать 11811, что примерно соответствует 300 DPI.
    int32_t ver_pix_per_m = 11811;
    //Количество использованных цветов — 4 байта, знаковое целое. Нужно записать 0 — значение не определено.
    int32_t used_color = 0;
    //Количество значимых цветов — 4 байта, знаковое целое. Нужно записать 0x1000000.
    int32_t sign_color = 0x1000000;
    BitmapInfoHeader() = default;
    BitmapInfoHeader(int32_t w, int32_t h);
}
PACKED_STRUCT_END

bool SaveBMP(const Path& file, const Image& image);
Image LoadBMP(const Path& file);

} // namespace img_lib
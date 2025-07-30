#pragma once
#include <iostream>
#include <string>


constexpr uint8_t MAX_SINGLE_COLOR_VALUE = UINT8_MAX;    // 0xFF(255)

// 明度の基準値
constexpr uint8_t DEFAULT_ALPHA_VALUE = 200;                // 標準色(原色だとまぶしいので明度の標準値は若干低めにしたい)
constexpr uint8_t MAX_ALPHA_VALUE = MAX_SINGLE_COLOR_VALUE; // 最大色

// 色情報の定義
using Color = uint32_t;

// R, G, B, A (0～255) を1つのuint32_tにまとめる (RGBA順)
constexpr Color MakeColorCode(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = DEFAULT_ALPHA_VALUE) noexcept
{
    return 
        (static_cast<uint32_t>(red)     << 24)  |
        (static_cast<uint32_t>(green)   << 16)  |
        (static_cast<uint32_t>(blue)    << 8 )  |
        (static_cast<uint32_t>(alpha))  << 0;
}


// enumに直接RGBA値を割り当て
enum PRINT_COLOR
{
    DEFAULT         = 0,
    BLACK           = MakeColorCode(0  , 0  , 0  ),
    RED             = MakeColorCode(255, 0  , 0  ),
    GREEN           = MakeColorCode(0  , 255, 0  ),
    YELLOW          = MakeColorCode(255, 255, 0  ),
    BLUE            = MakeColorCode(0  , 0  , 255),
    MAGENTA         = MakeColorCode(255, 0  , 255),
    CYAN            = MakeColorCode(0  , 255, 255),
    WHITE           = MakeColorCode(255, 255, 255),
    BRIGHT_BLACK    = MakeColorCode(64 , 64 , 64 , MAX_ALPHA_VALUE),
    BRIGHT_RED      = MakeColorCode(255, 128, 128, MAX_ALPHA_VALUE),
    BRIGHT_GREEN    = MakeColorCode(128, 255, 128, MAX_ALPHA_VALUE),
    BRIGHT_YELLOW   = MakeColorCode(255, 255, 128, MAX_ALPHA_VALUE),
    BRIGHT_BLUE     = MakeColorCode(128, 128, 255, MAX_ALPHA_VALUE),
    BRIGHT_MAGENTA  = MakeColorCode(255, 128, 255, MAX_ALPHA_VALUE),
    BRIGHT_CYAN     = MakeColorCode(128, 255, 255, MAX_ALPHA_VALUE),
    BRIGHT_WHITE    = MakeColorCode(255, 255, 255, MAX_ALPHA_VALUE)
};


// RGBで指定した色の文字列を生成(True Color対応)
inline std::string MakeColorCode(Color color = PRINT_COLOR::DEFAULT)
{
    uint8_t alpha = static_cast<uint8_t>((color >> 0)  & 0xFF);

    // アルファの値が0なら
    // 標準色を返す
    if (alpha == 0) 
        return "\033[0m";


    float alphaRatio = (float)alpha / 255;

    uint8_t blue  = static_cast<uint8_t>((color >> 8)  & 0xFF)  * alphaRatio;
    uint8_t green = static_cast<uint8_t>((color >> 16) & 0xFF)  * alphaRatio;
    uint8_t red   = static_cast<uint8_t>((color >> 24) & 0xFF)  * alphaRatio;


    return "\033[38;2;" + 
        std::to_string(red)     + ";" +
        std::to_string(green)   + ";" +
        std::to_string(blue)    + "m";
}



// RGBの色を直接指定して作成できるマクロ
#define PRINT_COLOR(red, green, blue) MakeColorCode(red, green, blue, MAX_ALPHA_VALUE)
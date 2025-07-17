#include "PrintFunction.hpp"
#include <iostream>


#if defined(_WIN32)
#include <windows.h>  // Windows API の型や関数の定義
#include <io.h>       // _isatty の定義
#else
#include <unistd.h>   // isatty の定義
#include <cstdlib>    // getenv
#endif

// 文字カラー出力が使えるかの確認フラグ
static bool g_colorOutputAvailable = isColorOutputAvailable();


bool isColorOutputAvailable()
{
#if defined(_WIN32)
    if (!_isatty(_fileno(stdout)))
    {
        return false;
    }

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return false;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return false;

    DWORD newMode = dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, newMode))
    {
        return false;
    }
    // 元に戻す(副作用防止)
    SetConsoleMode(hOut, dwMode);
    return true;

#else
    if (!isatty(STDOUT_FILENO))
    {
        return false;
    }

    const char* term = std::getenv("TERM");
    if (term == nullptr) return false;

    std::string termStr(term);
    if (termStr == "dumb")
    {
        return false;
    }

    return true;
#endif
}

// 色コードシーケンスだけ作る関数
std::string makeColorCode(PRINT_COLOR color)
{
    return "\033[" + std::to_string(static_cast<int>(color)) + "m";
}

// 標準出力ストリーム
void PrintMessage(const std::string& message, PRINT_COLOR color)
{
    if (g_colorOutputAvailable == true) // カラーの文字出力が有効
    {
        std::cout << makeColorCode(color) << message << makeColorCode(PRINT_COLOR::DEFAULT) << std::flush;
    }
    else                                // カラーの文字出力が無効
    {
        std::cout << message << std::flush;
    }

}

// 標準エラー出力ストリーム
void PrintErrorMessage(const std::string& message, PRINT_COLOR color)
{
    if (g_colorOutputAvailable == true) // カラーの文字出力が有効
    {
        std::cerr << makeColorCode(color) << message << makeColorCode(PRINT_COLOR::DEFAULT) << std::flush;
    }
    else                                // カラーの文字出力が無効
    {
        std::cerr << message << std::flush;
    }

}
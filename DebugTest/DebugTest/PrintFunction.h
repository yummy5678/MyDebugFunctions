#pragma once
#include <iostream>
#include <string>

#if defined(_WIN32)
#include <windows.h>  // Windows API の型や関数の定義
#include <io.h>       // _isatty の定義
#else
#include <unistd.h>   // isatty の定義
#include <cstdlib>    // getenv
#endif
#include "ColorDefine.h"



// 文字出力関数のマクロ版
#define PRINT_MESSAGE(message) DebugPrint::PrintMessage(message)
#define PRINT_MESSAGE_COLOR(message, color) DebugPrint::PrintMessage(message, color)

#define PRINT_ERROR_MESSAGE(message) DebugPrint::PrintErrorMessage(message)
#define PRINT_ERROR_MESSAGE_COLOR(message, color) DebugPrint::PrintErrorMessage(message, color)

namespace DebugPrint
{

    // 文字カラー出力が使えるか確認する関数
    inline bool IsColorOutputAvailable()
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


    // 文字カラー出力が使えるかの確認フラグ
    static bool g_colorOutputAvailable = IsColorOutputAvailable();

    // 標準出力ストリーム
    inline void PrintMessage(const std::string& message, Color color = PRINT_COLOR::DEFAULT)
    {
        if (g_colorOutputAvailable == true) // カラーの文字出力が有効
        {
            std::cout << MakeColorCode(color) << message << MakeColorCode(PRINT_COLOR::DEFAULT) << std::flush;
        }
        else                                // カラーの文字出力が無効
        {
            std::cout << message << std::flush;
        }

    }


    // 標準エラー出力ストリーム
    inline void PrintErrorMessage(const std::string& message, Color color = PRINT_COLOR::YELLOW)
    {
        if (g_colorOutputAvailable == true) // カラーの文字出力が有効
        {
            std::cerr << MakeColorCode(color) << message << MakeColorCode(PRINT_COLOR::DEFAULT) << std::flush;
        }
        else                                // カラーの文字出力が無効
        {
            std::cerr << message << std::flush;
        }

    }


}
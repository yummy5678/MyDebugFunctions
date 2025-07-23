#pragma once
#include <string>
#include <iostream>

// 文字出力関数のマクロ版
#define PRINT_MESSAGE(message) PrintMessage(message)
#define PRINT_MESSAGE_COLOR(message, color) PrintMessage(message, color)

#define PRINT_ERROR_MESSAGE(message) PrintErrorMessage(message)
#define PRINT_ERROR_MESSAGE_COLOR(message, color) PrintErrorMessage(message, color)


// 簡易カラー表
enum class PRINT_COLOR
{
    DEFAULT = 0,
    BLACK   = 30,
    RED     = 31,
    GREEN   = 32,
    YELLOW  = 33,
    BLUE    = 34,
    MAGENTA = 35,
    CYAN    = 36,
    WHITE   = 37
};

// 文字カラー出力が使えるか確認する関数
bool isColorOutputAvailable();

// 色コードシーケンスだけ作る関数
std::string makeColorCode(PRINT_COLOR color);

// 標準出力ストリーム
void PrintMessage(const std::string& message, PRINT_COLOR color = PRINT_COLOR::DEFAULT);

// 標準エラー出力ストリーム
void PrintErrorMessage(const std::string& message, PRINT_COLOR color = PRINT_COLOR::YELLOW);


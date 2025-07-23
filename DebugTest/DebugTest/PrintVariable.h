#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <type_traits>
#include "PrintFunction.h"
#include "TemplateStrings.h"

// ===== 汎用表示 =====
#define PRINT_VARIABLE(variable) DebugPrint::PrintVariable(#variable, variable)
#define PRINT_VARIABLE_COLOR(variable, color) DebugPrint::PrintVariable(#variable, variable, color)

namespace DebugPrint
{
    // ===== 判定ヘルパー =====
    // C配列判定
    template <typename T>
    constexpr bool is_c_array = std::is_array<T>::value;

    // std::vector 判定
    template <typename T>
    struct is_vector : std::false_type {};

    template <typename T, typename A>
    struct is_vector<std::vector<T, A>> : std::true_type {};

    // std::array 判定
    template <typename T>
    struct is_std_array : std::false_type {};

    template <typename T, std::size_t N>
    struct is_std_array<std::array<T, N>> : std::true_type {};

    // 総合判定(コンテナかどうか)
    template <typename T>
    struct is_array_like : std::disjunction<
        std::is_array<T>,
        is_vector<T>,
        is_std_array<T>
    > {
    };

    // 通常型(配列以外)
    template <typename T>
    std::enable_if_t<!is_array_like<T>::value>  // std::enable_if を使った「SFINAE(条件付き関数有効化)
        PrintVariable(const char* name, const T& var, PRINT_COLOR color = PRINT_COLOR::DEFAULT)
    {
        std::ostringstream outputString;
        // 変数: [変数名]  値: [変数の中身]
        outputString << variableString << pairSeparatorString << name << "  ";
        outputString << valueString << pairSeparatorString << var << "\n";
        PrintMessage(outputString.str(), color);  // 色つき出力
    }

    // C配列
    template <typename T, std::size_t N>  
    void PrintVariable(const std::string& varName, const T(&arr)[N], PRINT_COLOR color = PRINT_COLOR::DEFAULT)
    {
        std::ostringstream outputString;

        // 変数名 [ 要素数 ] :(改行)
        outputString << variableString << varName
            << openString << N << closeString << pairSeparatorString << "\n";
        for (size_t i = 0; i < N; ++i)
        {
            // [表示番号] : 数値 (改行)
            outputString << openString << i << closeString << pairSeparatorString << arr[i] << "\n";
        }

        PrintMessage(outputString.str(), color);  // 最終結果を出力
    }


    // std::vector / std::array 共通
    template <typename Container>
    std::enable_if_t<is_array_like<Container>::value && !std::is_array<Container>::value>
        PrintVariable(const std::string& varName, const Container& container, PRINT_COLOR color = PRINT_COLOR::DEFAULT)
    {
        std::ostringstream outputString;

        // 変数名 [ 要素数 ] :(改行)
        outputString << variableString << varName
            << openString << container.size() << closeString << pairSeparatorString << "\n";

        size_t index = 0;
        for (const auto& value : container)
        {
            // [要素番号] : 数値 (改行)
            outputString << openString << index++ << closeString << pairSeparatorString << value << "\n";
        }

        PrintMessage(outputString.str(), color);  // 最終結果を出力
    }

}




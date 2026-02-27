#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <type_traits>
#include "PrintFunction.h"
#include "TemplateStrings.h"
#include "magic_enum/magic_enum.hpp"


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

    // 列挙型(enum / enum class)の場合 → magic_enum で文字列に変換して表示
    template <typename T>
    std::enable_if_t<std::is_enum<T>::value>
        PrintVariable(const char* name, const T& var, Color color = PRINT_COLOR::DEFAULT)
    {
        std::ostringstream outputString;
        outputString << variableString() << pairSeparatorString() << name << "  ";
        outputString << valueString() << pairSeparatorString() << magic_enum::enum_name(var) << "\n";
        PrintMessage(outputString.str(), color);
    }

    // 通常型(配列・列挙型以外)
    template <typename T>
    std::enable_if_t<!is_array_like<T>::value && !std::is_enum<T>::value>
        PrintVariable(const char* name, const T& var, Color color = PRINT_COLOR::DEFAULT)
    {
        static_assert(requires(std::ostream & os, const T & v) { os << v; },
            "PRINT_VARIABLE は << 演算子が定義されていない型には使用できません。"
            "構造体の場合は PRINT_STRUCT を使用してください");

        std::ostringstream outputString;
        outputString << variableString() << pairSeparatorString() << name << "  ";
        outputString << valueString() << pairSeparatorString() << var << "\n";
        PrintMessage(outputString.str(), color);
    }

    // C配列
    template <typename T, std::size_t N>
    void PrintVariable(const std::string& varName, const T(&arr)[N], Color color = PRINT_COLOR::DEFAULT)
    {
        std::ostringstream outputString;
        outputString << variableString() << varName
            << openString() << N << closeString() << pairSeparatorString() << "\n";
        for (size_t i = 0; i < N; ++i)
        {
            outputString << openString() << i << closeString() << pairSeparatorString() << arr[i] << "\n";
        }
        PrintMessage(outputString.str(), color);
    }

    // std::vector / std::array 共通
    template <typename Container>
    std::enable_if_t<is_array_like<Container>::value && !std::is_array<Container>::value>
        PrintVariable(const std::string& varName, const Container& container, Color color = PRINT_COLOR::DEFAULT)
    {
        std::ostringstream outputString;
        outputString << variableString() << varName
            << openString() << container.size() << closeString() << pairSeparatorString() << "\n";

        size_t index = 0;
        for (const auto& value : container)
        {
            outputString << openString() << index++ << closeString() << pairSeparatorString() << value << "\n";
        }
        PrintMessage(outputString.str(), color);
    }
}
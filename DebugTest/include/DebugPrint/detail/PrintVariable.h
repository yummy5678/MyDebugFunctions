#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <type_traits>
#include "PrintFunction.h"
#include "TemplateStrings.h"
#include "../third_party/magic_enum/magic_enum.hpp"

namespace DebugPrint
{
    // ===== 型判定ヘルパー =====

    /// @brief C配列かどうかを判定する
    template <typename T>
    constexpr bool is_c_array = std::is_array<T>::value;

    /// @brief std::vector かどうかを判定する
    template <typename T>
    struct is_vector : std::false_type {};

    template <typename T, typename A>
    struct is_vector<std::vector<T, A>> : std::true_type {};

    /// @brief std::array かどうかを判定する
    template <typename T>
    struct is_std_array : std::false_type {};

    template <typename T, std::size_t N>
    struct is_std_array<std::array<T, N>> : std::true_type {};

    /// @brief C配列・std::vector・std::array のいずれかかどうかを判定する
    template <typename T>
    struct is_array_like : std::disjunction<
        std::is_array<T>,
        is_vector<T>,
        is_std_array<T>
    > {
    };

    /// @brief 列挙型変数の名前と値をコンソールに表示する。
    /// magic_enum を使って列挙値を文字列に変換して表示する。
    /// PRINT_VARIABLE マクロから呼び出される
    /// @param name 変数名の文字列
    /// @param var 表示する列挙型変数
    /// @param color 表示色
    template <typename T>
    std::enable_if_t<std::is_enum<T>::value>
        PrintVariable(const char* name, const T& var, Color color = PRINT_COLOR::DEFAULT)
    {
        std::ostringstream outputString;
        outputString << variableString() << pairSeparatorString() << name << "  ";
        outputString << valueString() << pairSeparatorString() << magic_enum::enum_name(var) << "\n";
        PrintMessage(outputString.str(), color);
    }

    /// @brief 通常型変数の名前と値をコンソールに表示する。
    /// 配列型・列挙型以外の << 演算子が定義された型に対応する。
    /// PRINT_VARIABLE マクロから呼び出される
    /// @param name 変数名の文字列
    /// @param var 表示する変数
    /// @param color 表示色
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

    /// @brief C配列の変数名・要素数・各要素の値をコンソールに表示する。
    /// PRINT_VARIABLE マクロから呼び出される
    /// @param varName 変数名の文字列
    /// @param arr 表示するC配列
    /// @param color 表示色
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

    /// @brief std::vector または std::array の変数名・要素数・各要素の値をコンソールに表示する。
    /// PRINT_VARIABLE マクロから呼び出される
    /// @param varName 変数名の文字列
    /// @param container 表示するコンテナ
    /// @param color 表示色
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

} // namespace DebugPrint
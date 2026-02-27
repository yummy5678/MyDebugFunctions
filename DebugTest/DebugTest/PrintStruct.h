#pragma once
#include <sstream>
#include <string>
#include <type_traits>
#include <boost/pfr.hpp>
#include "PrintFunction.h"
#include "TemplateStrings.h"
#include "PrintVariable.h"
#include "magic_enum/magic_enum.hpp"

namespace DebugPrint
{
    // 前方宣言
    template <typename T>
    void PrintStructMembers(const T& var, Color color, int indent);

    // Boost.PFR で扱える集成体かどうかの判定ヘルパー。
    // 集成体 = 継承なし・仮想関数なし・ユーザー定義コンストラクタなし
    template <typename T>
    struct is_reflectable
    {
        static constexpr bool value =
            std::is_aggregate<T>::value &&
            !is_array_like<T>::value &&
            !std::is_array<T>::value;
    };

    /// @brief 構造体のメンバーを再帰的に表示するヘルパー。
    /// Boost.PFR でメンバーを走査し、メンバーが集成体の場合は再帰的に表示する。
    /// インデントレベルに応じてスペースを挿入して階層構造を表現する
    /// @param var 表示する構造体
    /// @param color 表示色
    /// @param indent 現在のインデントレベル
    template <typename T>
    void PrintStructMembers(const T& var, Color color, int indent)
    {
        // インデント用のスペース(1レベルにつき2スペース)
        const std::string indentStr(indent * 2, ' ');

        // 全メンバー名を配列で取得する(C++20 + Boost 1.84以降)
        constexpr auto memberNames = boost::pfr::names_as_array<T>();

        boost::pfr::for_each_field(var, [&]<typename Field>(const Field & field, std::size_t idx)
        {
            // std::string_view を std::string に変換して出力できるようにする
            const std::string memberName(memberNames[idx]);

            std::ostringstream out;

            if constexpr (is_reflectable<Field>::value)
            {
                out << indentStr
                    << variableString() << pairSeparatorString() << memberName << "\n";
                PrintMessage(out.str(), color);
                PrintStructMembers(field, color, indent + 1);
            }
            else if constexpr (std::is_enum<Field>::value)
            {
                out << indentStr
                    << variableString() << pairSeparatorString() << memberName << "  "
                    << valueString() << pairSeparatorString() << magic_enum::enum_name(field) << "\n";
                PrintMessage(out.str(), color);
            }
            else
            {
                out << indentStr
                    << variableString() << pairSeparatorString() << memberName << "  "
                    << valueString() << pairSeparatorString() << field << "\n";
                PrintMessage(out.str(), color);
            }
        });
    }

    /// @brief 構造体名とメンバーをコンソールに表示する。
    /// Boost.PFR を使ってメンバーを自動取得し、ネストした集成体は再帰的に表示する。
    /// PRINT_STRUCT マクロから呼び出される
    /// @param name 構造体変数名の文字列
    /// @param var 表示する構造体
    /// @param color 表示色
    /// @param indent インデントレベル(通常は0)
    template <typename T>
    void PrintStruct(const char* name, const T& var, Color color = PRINT_COLOR::DEFAULT, int indent = 0)
    {
        // 継承クラスを入れようとしたらエラーになる。
        // この関数にはシンプルな構造体を入れて使って欲しい。
        static_assert(is_reflectable<T>::value,
            "PRINT_STRUCT は集成体(継承なし・仮想関数なし・ユーザー定義コンストラクタなし)のみ対応しています");

        std::ostringstream out;
        out << separatorString()
            << variableString() << pairSeparatorString() << name << "\n";
        PrintMessage(out.str(), color);
        PrintStructMembers(var, color, indent);
        PrintMessage(separatorString(), color);
    }

} // namespace DebugPrint
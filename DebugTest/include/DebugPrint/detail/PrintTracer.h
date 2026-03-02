#pragma once
#include "MacroList.h"
#include "PrintFunction.h"
#include "TemplateStrings.h"
#include "TimeUtility.h"

namespace DebugPrint
{
    /// @brief PRINT_TRACE_FUNCTION マクロから使用される、関数の開始・終了と経過時間を出力するクラス。
    /// コンストラクタで関数の開始情報を出力し、デストラクタで終了情報と経過時間を出力する。
    /// スコープを抜けると自動的にデストラクタが呼ばれるため、関数の先頭に置くだけで使用できる
    class FunctionTracer
    {
    public:
        /// @brief デフォルトコンストラクタは使用禁止
        FunctionTracer() = delete;
        /// @brief コピーコンストラクタは使用禁止
        FunctionTracer(FunctionTracer const&) = delete;
        /// @brief コピー代入演算子は使用禁止
        FunctionTracer& operator=(const FunctionTracer&) = delete;

        /// @brief 関数の開始情報を出力するコンストラクタ
        /// @param func_name 呼び出し元の関数名
        /// @param file_name 呼び出し元のファイル名
        /// @param line_number 呼び出し元の行番号
        /// @param color 表示色
        FunctionTracer(const char func_name[], const char file_name[], int line_number, Color color = PRINT_COLOR::DEFAULT) noexcept
        {
            m_Timer.Start();
            m_Color = color;
            std::string functionName = func_name;
            std::string fileName = file_name;

            PrintMessage(separatorString(), m_Color);
            PrintMessage(GetDateTimeString() + "\n", m_Color);
            PrintMessage(fileString() + pairSeparatorString() + fileName + "\n" +
                LineNumberString() + pairSeparatorString() +
                std::to_string(line_number) + "\n", m_Color);
            PrintMessage(functionName, m_Color);
            PrintMessage(startFunctionString(), m_Color);
        }

        /// @brief 関数の終了情報と経過時間を出力するデストラクタ
        ~FunctionTracer() noexcept
        {
            PrintMessage(endFunctionString(), m_Color);
            PrintMessage(endTimerString(), m_Color);
            PrintMessage(m_Timer.GetElapsedSecTime(), m_Color);
            PrintMessage(secondsString() + "\n", m_Color);
            PrintMessage(separatorString(), m_Color);
        }

    private:
        Color m_Color;  // 表示色
        Timer m_Timer;  // 経過時間計測用タイマー
    };


    /// @brief PRINT_TRACE_CLASS マクロから使用される、クラスのコンストラクタ・デストラクタの呼び出しを出力するクラス。
    /// コンストラクタ呼び出し時に開始メッセージを、デストラクタ呼び出し時に終了メッセージを出力する。
    /// メンバ変数として宣言することでクラスのライフサイクルを追跡できる
    class ClassTracer
    {
    public:
        /// @brief クラスの開始メッセージを出力するコンストラクタ
        /// @param class_name 追跡するクラス名
        /// @param color 表示色
        ClassTracer(const char class_name[], Color color = PRINT_COLOR::GREEN)
        {
            m_ClassName = class_name;
            m_Color = color;
            PrintMessage(m_ClassName + startClassString(), m_Color);
        }

        /// @brief クラスの終了メッセージを出力するデストラクタ
        ~ClassTracer()
        {
            PrintMessage(m_ClassName + endClassString(), m_Color);
        }

    private:
        std::string m_ClassName;  // 追跡するクラス名
        Color       m_Color;      // 表示色
    };

} // namespace DebugPrint
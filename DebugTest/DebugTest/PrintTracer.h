#pragma once
#include "MacroList.h"
#include "PrintFunction.h"
#include "TemplateStrings.h"
#include "TimeUtility.h"

// 関数の導入から終了までを出力するマクロ
#define PRINT_TRACE_FUNCTION                DebugPrint::FunctionTracer _funcInfo(THIS_FUNCTION_NAME, THIS_FILE_NAME, THIS_LINE_NUMBER)
#define PRINT_TRACE_FUNCTION_COLOR(color)   DebugPrint::FunctionTracer _funcColorInfo(THIS_FUNCTION_NAME, THIS_FILE_NAME, THIS_LINE_NUMBER, color)

// 関数の導入から終了までを出力するマクロ
#define PRINT_TRACE_CLASS(name)                 DebugPrint::ClassTracer _classTracer{#name}
#define PRINT_TRACE_CLASS_COLOR(name, color)    DebugPrint::ClassTracer _classColorTracer{#name, color}



namespace DebugPrint
{
    class FunctionTracer
    {
    public:
        // コンストラクタの設定
        FunctionTracer() = delete;
        FunctionTracer(FunctionTracer const&) = delete;

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


        ~FunctionTracer() noexcept
        {
            PrintMessage(endFunctionString(), m_Color);
            PrintMessage(endTimerString(), m_Color);
            PrintMessage(m_Timer.GetElapsedSecTime(), m_Color);
            PrintMessage(secondsString() + "\n", m_Color);
            PrintMessage(separatorString(), m_Color);
        }

        FunctionTracer& operator =(const FunctionTracer&) = delete;


    private:
        Color m_Color;
        Timer m_Timer;
    };




    class ClassTracer
    {
    public:
        ClassTracer(const char class_name[], Color color = PRINT_COLOR::GREEN)
        {
            m_ClassName = class_name;
            m_Color = color;
            PrintMessage(m_ClassName + startClassString(), m_Color);
        }

        ~ClassTracer()
        {
            PrintMessage(m_ClassName + endClassString(), m_Color);
        }

        std::string m_ClassName;
        Color m_Color;

    };
}

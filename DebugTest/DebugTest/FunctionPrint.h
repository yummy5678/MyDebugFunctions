#pragma once
#include "Macro.h"
#include "PrintFunction.h"
#include "TemplateStrings.h"
#include "DebugUtility.h"


#define DEBUG_FUNCTION_INFO DebugUtility::FunctionCommentator _funcInfo(THIS_FUNCTION_NAME, THIS_FILE_NAME, THIS_LINE_NUMBER)



namespace DebugUtility
{
    class FunctionCommentator
    {
    public:
        // コンストラクタの設定
        FunctionCommentator() = delete;
        FunctionCommentator(FunctionCommentator const&) = delete;

        FunctionCommentator(const char func_name[], const char file_name[], int line_number, PRINT_COLOR color = PRINT_COLOR::DEFAULT) noexcept
        {
            m_Timer.Start();
            m_Color = color;
            std::string functionName = func_name;
            std::string fileName = file_name;
           
            PrintMessage(separatorString, m_Color);
            PrintMessage(GetDateTimeString() + "\n", m_Color);
            PrintMessage(fileString + pairSeparatorString + fileName + "\n" +
                LineNumberString + pairSeparatorString +
                std::to_string(line_number) + "\n", m_Color);
            PrintMessage(functionName, m_Color);
            PrintMessage(startFunctionString, m_Color);

        }


        ~FunctionCommentator() noexcept 
        {
            PrintMessage(endFunctionString, m_Color);

            PrintMessage(endTimerString, m_Color);
            PrintMessage(m_Timer.GetElapsedSecTime(), m_Color);
            PrintMessage(secondsString + "\n", m_Color);

            PrintMessage(separatorString, m_Color);
        }

        FunctionCommentator& operator =(const FunctionCommentator&) = delete;


    private:
        PRINT_COLOR m_Color;
        Timer m_Timer;
    };


}

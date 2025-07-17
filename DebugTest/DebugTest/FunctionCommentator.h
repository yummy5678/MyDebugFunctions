#pragma once
#include "Macro.h"
#include "PrintFunction.hpp"
#include "TemplateStrings.h"
#include "DebugUtility.h"


#define DEBUG_FUNCTION_START DebugUtility::FunctionCommentator _funcInfo(THIS_FUNCTION_NAME, THIS_FILE_NAME, THIS_LINE_NUMBER)



namespace DebugUtility
{
    class FunctionCommentator
    {
    public:
        // コンストラクタの設定
        FunctionCommentator() = delete;
        FunctionCommentator(FunctionCommentator const&) = delete;

        FunctionCommentator(const char func_name[], const char file_name[], int line_number) noexcept
        {
            m_Timer.Start();
            std::string functionName = func_name;
            std::string fileName = file_name;
           
            PrintMessage(separatorString);
            PrintMessage(GetDateTimeString() + "\n");
            PrintMessage(FileString + fileName + " ," + LineNumberString + std::to_string(line_number) + "\n");
            PrintMessage(functionName);
            PrintMessage(startFunctionString);

        }


        ~FunctionCommentator() noexcept 
        {
            PrintMessage(endFunctionString);

            PrintMessage(endTimerString);
            PrintMessage(m_Timer.GetElapsedSecTime());
            PrintMessage(secondsString + "\n");

            PrintMessage(separatorString);
        }

        FunctionCommentator& operator =(const FunctionCommentator&) = delete;


    private:
        Timer m_Timer;
    };
}

#pragma once
#include <cstdlib>
#include <iostream>
#include "DebugPrint.h"

class HogeClass
{
public:
    HogeClass() = default;
    ~HogeClass() = default;

private:
    PRINT_TRACE_CLASS(HogeClass);
};


int main(int argc, char* argv[])
{
    PRINT_TRACE_FUNCTION;

    HogeClass hoge;

    DebugPrint::PrintMessage("PrintMessage(Default)\n");
    DebugPrint::PrintMessage("PrintMessage(Color)\n", PRINT_COLOR::BLUE);
    DebugPrint::PrintErrorMessage("ErrorMessage(Default)\n");
    DebugPrint::PrintErrorMessage("ErrorMessage(Color)\n", PRINT_COLOR::RED);

    int a = 10;
    int arr[] = { 1, 2, 3 };
    std::vector<std::string> vec = { "Alice", "Bob" };
    std::array<float, 3> ary = { 3.14f, 2.71f, 1.61f };

    PRINT_VARIABLE(a);    // a = 10
    PRINT_VARIABLE(arr);  // arr = [1, 2, 3]
    PRINT_VARIABLE_COLOR(vec, PRINT_COLOR::WHITE);  // vec = [Alice, Bob]
    PRINT_VARIABLE_COLOR(ary, PRINT_COLOR::BRIGHT_WHITE);  // ary = [3.14, 2.71, 1.61]

    PRINT_MESSAGE_COLOR("マクロ版メッセージ表示\n", PRINT_COLOR::MAGENTA);
    PRINT_MESSAGE("マクロ版メッセージ表示\n");
    PRINT_ERROR_MESSAGE_COLOR("マクロ版メッセージ表示\n", PRINT_COLOR::BRIGHT_MAGENTA);
    PRINT_MESSAGE("マクロ版メッセージ表示\n",0,0,0);

    PRINT_TRACE_FUNCTION_COLOR(PRINT_COLOR(100,100,0));

    return EXIT_SUCCESS;
}
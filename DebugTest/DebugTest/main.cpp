#pragma once
#include <cstdlib>
#include <iostream>
#include "DebugPrint.h"




int main(int argc, char* argv[])
{
    DEBUG_FUNCTION_INFO;

    PrintMessage("PrintMessage(Default)\n");
    PrintMessage("PrintMessage(Color)\n", PRINT_COLOR::BLUE);
    PrintErrorMessage("ErrorMessage(Default)\n");
    PrintErrorMessage("ErrorMessage(Color)\n", PRINT_COLOR::RED);

    int a = 10;
    int arr[] = { 1, 2, 3 };
    std::vector<std::string> vec = { "Alice", "Bob" };
    std::array<float, 3> ary = { 3.14f, 2.71f, 1.61f };

    PRINT_VARIABLE(a);    // a = 10
    PRINT_VARIABLE(arr);  // arr = [1, 2, 3]
    PRINT_VARIABLE_COLOR(vec, PRINT_COLOR::BLUE);  // vec = [Alice, Bob]
    PRINT_VARIABLE(ary);  // ary = [3.14, 2.71, 1.61]

    PRINT_MESSAGE_COLOR("マクロ版メッセージ表示\n", PRINT_COLOR::MAGENTA);
    PRINT_MESSAGE("マクロ版メッセージ表示\n");
    PRINT_ERROR_MESSAGE_COLOR("マクロ版メッセージ表示\n", PRINT_COLOR::GREEN);
    PRINT_ERROR_MESSAGE("マクロ版メッセージ表示\n");

    DEBUG_FUNCTION_INFO_COLOR(PRINT_COLOR::BLUE);

    return EXIT_SUCCESS;
}
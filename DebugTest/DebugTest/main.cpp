#pragma once
#include <cstdlib>
#include <iostream>
#include "PrintFunction.hpp"
#include "FunctionCommentator.h"



int main(int argc, char* argv[])
{
    DEBUG_FUNCTION_START;

    PrintMessage("PrintMessage(Default)\n");
    PrintMessage("PrintMessage(Color)\n", PRINT_COLOR::BLUE);
    PrintErrorMessage("ErrorMessage(Default)\n");
    PrintErrorMessage("ErrorMessage(Color)\n", PRINT_COLOR::RED);
    return EXIT_SUCCESS;
}
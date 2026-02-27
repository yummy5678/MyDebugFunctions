#pragma once
#include "DebugPrintConfig.h"

// このマクロを定義するとデバッグ出力ライブラリ全体が有効になる
// 定義しない場合はすべてのマクロが空になり、コードから除去される
#define DEBUG_PRINT_ENABLED

#ifdef DEBUG_PRINT_ENABLED

#include "PrintMacroList.h"
#include "PrintTracer.h"
#include "PrintVariable.h"

#else

// ライブラリ無効時は全マクロを空にする
// ライブラリ無効時は全マクロを空にする
#define PRINT_MESSAGE(message)
#define PRINT_WARNING_MESSAGE(message)
#define PRINT_ERROR_MESSAGE(message)
#define POPUP_MESSAGE(message)
#define POPUP_MESSAGE_ICON(message, icon)
#define POPUP_WARNING_MESSAGE(message)
#define POPUP_ERROR_MESSAGE(message)
#define PRINT_VARIABLE(variable)
#define PRINT_STRUCT(variable)
#define PRINT_TRACE_FUNCTION
#define PRINT_TRACE_CLASS(name)
#define DEBUG_SET_LANGUAGE(lang)
#define DEBUG_SET_DATETIME_FORMAT(format)
#define DEBUG_SET_LOG_PATH(path)
#define DEBUG_SET_EXIT_ON_ERROR(enabled)
#define DEBUG_SET_PRINT_MESSAGE_COLOR(color)
#define DEBUG_SET_PRINT_WARNING_MESSAGE_COLOR(color)
#define DEBUG_SET_PRINT_ERROR_MESSAGE_COLOR(color)
#define DEBUG_SET_POPUP_MESSAGE_COLOR(color)
#define DEBUG_SET_POPUP_WARNING_MESSAGE_COLOR(color)
#define DEBUG_SET_POPUP_ERROR_MESSAGE_COLOR(color)
#define DEBUG_WRITE_LOG(path)
#define DEBUG_CLEAR_LOG()

#endif
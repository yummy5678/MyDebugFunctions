#pragma once
#include <cstdlib>
#include "MacroList.h"
#include "PrintVariable.h"
#include "PrintStruct.h"
#include "PrintFunction.h"


// 変数表示マクロ
#define PRINT_VARIABLE(variable) \
    DebugPrint::PrintVariable(#variable, variable)

// 構造体のメンバーを表示するマクロ
#define PRINT_STRUCT(variable) \
    DebugPrint::PrintStruct(#variable, variable)

// メッセージ表示マクロ
#define PRINT_MESSAGE(message) \
    DebugPrint::PrintMessage(message)

// 警告メッセージ表示マクロ。コンソールに警告情報を表示する。アプリは継続する
#define PRINT_WARNING_MESSAGE(message) \
    DebugPrint::PrintAppWarningMessage(message, THIS_FUNCTION_NAME, THIS_FILE_NAME, THIS_LINE_NUMBER)

// エラーメッセージ表示マクロ。コンソールにエラー情報を表示する。
// DebugPrintConfig::SetExitOnError(true) の場合はアプリを終了する
#define PRINT_ERROR_MESSAGE(message) \
    DebugPrint::PrintAppErrorMessage(message, THIS_FUNCTION_NAME, THIS_FILE_NAME, THIS_LINE_NUMBER)

// ポップアップ表示マクロ。コンソールとポップアップにメッセージを表示する。アプリは継続する
#define POPUP_MESSAGE(message) \
    DebugPrint::ShowPopupMessage(message, DebugPrint::PopupIcon::None)

// ポップアップ表示マクロ(アイコン指定版)
#define POPUP_MESSAGE_ICON(message, icon) \
    DebugPrint::ShowPopupMessage(message, icon)

// 警告ポップアップ表示マクロ。コンソールと警告アイコン付きポップアップを表示する。アプリは継続する
#define POPUP_WARNING_MESSAGE(message) \
    DebugPrint::ShowPopupWarningMessage(message, THIS_FUNCTION_NAME, THIS_FILE_NAME, THIS_LINE_NUMBER)

// エラーポップアップ表示マクロ。コンソールとエラーアイコン付きポップアップを表示する。
// DebugPrintConfig::SetExitOnError(true) の場合はアプリを終了する
#define POPUP_ERROR_MESSAGE(message) \
    DebugPrint::ShowPopupErrorMessage(message, THIS_FUNCTION_NAME, THIS_FILE_NAME, THIS_LINE_NUMBER)



// ===== Config 設定マクロ =====

// 言語を切り替えるマクロ
#define DEBUG_SET_LANGUAGE(lang) \
    DebugPrint::DebugPrintConfig::GetInstance().SetLanguage(lang)

// 日時フォーマットを変更するマクロ
#define DEBUG_SET_DATETIME_FORMAT(format) \
    DebugPrint::DebugPrintConfig::GetInstance().SetDatetimeFormat(format)

// ログファイルの出力先パスを設定するマクロ
#define DEBUG_SET_LOG_PATH(path) \
    DebugPrint::DebugPrintConfig::GetInstance().SetLogPath(path)

// エラー系マクロ呼び出し時の終了有無を設定するマクロ
#define DEBUG_SET_EXIT_ON_ERROR(enabled) \
    DebugPrint::DebugPrintConfig::GetInstance().SetExitOnError(enabled)

// PRINT_MESSAGE の表示色を設定するマクロ
#define DEBUG_SET_PRINT_MESSAGE_COLOR(color) \
    DebugPrint::DebugPrintConfig::GetInstance().SetPrintMessageColor(color)

// PRINT_WARNING_MESSAGE の表示色を設定するマクロ
#define DEBUG_SET_PRINT_WARNING_MESSAGE_COLOR(color) \
    DebugPrint::DebugPrintConfig::GetInstance().SetPrintWarningMessageColor(color)

// PRINT_ERROR_MESSAGE の表示色を設定するマクロ
#define DEBUG_SET_PRINT_ERROR_MESSAGE_COLOR(color) \
    DebugPrint::DebugPrintConfig::GetInstance().SetPrintErrorMessageColor(color)

// POPUP_MESSAGE の表示色を設定するマクロ
#define DEBUG_SET_POPUP_MESSAGE_COLOR(color) \
    DebugPrint::DebugPrintConfig::GetInstance().SetPopupMessageColor(color)

// POPUP_WARNING_MESSAGE の表示色を設定するマクロ
#define DEBUG_SET_POPUP_WARNING_MESSAGE_COLOR(color) \
    DebugPrint::DebugPrintConfig::GetInstance().SetPopupWarningMessageColor(color)

// POPUP_ERROR_MESSAGE の表示色を設定するマクロ
#define DEBUG_SET_POPUP_ERROR_MESSAGE_COLOR(color) \
    DebugPrint::DebugPrintConfig::GetInstance().SetPopupErrorMessageColor(color)

// ログエントリをファイルに書き出すマクロ。パスを指定して書き出す
#define DEBUG_WRITE_LOG(path) \
    DebugPrint::LogWriter::GetInstance().WriteToFile(path)


// ログエントリを消去するマクロ
#define DEBUG_CLEAR_LOG() \
    DebugPrint::LogWriter::GetInstance().Clear()
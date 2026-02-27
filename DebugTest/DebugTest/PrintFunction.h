#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "tinyfiledialogs/tinyfiledialogs.h"
#include "ColorDefine.h"
#include "TemplateStrings.h"
#include "DebugPrintConfig.h"

namespace DebugPrint
{
    /// @brief ポップアップダイアログのアイコン種別を表す列挙型
    enum class PopupIcon
    {
        None,     ///< アイコンなし
        Info,     ///< 情報アイコン
        Warning,  ///< 警告アイコン
        Error,    ///< エラーアイコン
        Question, ///< 質問アイコン
    };

    /// @brief PopupIcon を tinyfiledialogs の文字列に変換する
    /// @param icon アイコン種別
    /// @return tinyfiledialogs に渡すアイコン文字列
    inline const char* PopupIconToString(PopupIcon icon)
    {
        switch (icon)
        {
        case PopupIcon::Info:     return "info";
        case PopupIcon::Warning:  return "warning";
        case PopupIcon::Error:    return "error";
        case PopupIcon::Question: return "question";
        default:                  return "";
        }
    }

    /// @brief メッセージを標準出力に表示する基本関数。
    /// 指定色でメッセージを表示する
    /// @param message 表示するメッセージ
    /// @param color 表示色
    inline void PrintMessage(const std::string& message, Color color = PRINT_COLOR::DEFAULT)
    {
        if (DebugPrintConfig::GetInstance().IsColorOutputEnabled())
        {
            std::cout << MakeColorCode(color) << message << MakeColorCode(PRINT_COLOR::DEFAULT) << std::flush;
        }
        else
        {
            std::cout << message << std::flush;
        }
    }

    /// @brief メッセージを標準エラー出力に表示する基本関数。
    /// 指定色でメッセージを表示する
    /// @param message 表示するメッセージ
    /// @param color 表示色
    inline void PrintErrMessage(const std::string& message, Color color = PRINT_COLOR::DEFAULT)
    {
        if (DebugPrintConfig::GetInstance().IsColorOutputEnabled())
        {
            std::cerr << MakeColorCode(color) << message << MakeColorCode(PRINT_COLOR::DEFAULT) << std::flush;
        }
        else
        {
            std::cerr << message << std::flush;
        }
    }

    /// @brief エラー・警告情報を標準エラー出力に表示する内部共通処理。
    /// ファイル名・行番号・関数名・メッセージをセパレータで囲んで指定色で表示する
    /// @param message 表示するメッセージ
    /// @param funcName 呼び出し元の関数名
    /// @param fileName 呼び出し元のファイル名
    /// @param lineNumber 呼び出し元の行番号
    /// @param color 表示色
    inline void PrintAppErrorInfo(
        const std::string& message,
        const char* funcName,
        const char* fileName,
        int lineNumber,
        Color color)
    {
        std::ostringstream out;
        out << separatorString()
            << fileString() << pairSeparatorString() << fileName << "\n"
            << LineNumberString() << pairSeparatorString() << lineNumber << "\n"
            << functionNameString() << pairSeparatorString() << funcName << "\n"
            << message << "\n"
            << separatorString();
        PrintErrMessage(out.str(), color);
    }

    /// @brief ポップアップダイアログを表示する内部共通処理。
    /// Emscripten ではブラウザの alert を使用し、それ以外は tinyfiledialogs を使用する
    /// @param message 表示するメッセージ
    /// @param icon アイコン種別
    inline void ShowPopup(const std::string& message, PopupIcon icon)
    {
#if defined(__EMSCRIPTEN__)
        EM_JS(void, js_alert, (const char* msg), {
            window.alert(UTF8ToString(msg));
            });
        js_alert(message.c_str());
#else
        tinyfd_messageBox(
            errorDialogTitle().c_str(),
            message.c_str(),
            "ok",
            PopupIconToString(icon),
            1
        );
#endif
    }

    /// @brief PRINT_WARNING_MESSAGE マクロから呼び出される警告表示処理。
    /// Config の PRINT_WARNING_MESSAGE 用の色でコンソールに表示する。アプリは継続する
    /// @param message 警告メッセージ
    /// @param funcName 呼び出し元の関数名
    /// @param fileName 呼び出し元のファイル名
    /// @param lineNumber 呼び出し元の行番号
    inline void PrintAppWarningMessage(
        const std::string& message,
        const char* funcName,
        const char* fileName,
        int lineNumber)
    {
        PrintAppErrorInfo(message, funcName, fileName, lineNumber,
            DebugPrintConfig::GetInstance().GetPrintWarningMessageColor());
    }

    /// @brief PRINT_ERROR_MESSAGE マクロから呼び出されるエラー表示処理。
    /// Config の PRINT_ERROR_MESSAGE 用の色でコンソールに表示する。
    /// IsExitOnError() が true の場合はアプリを終了する
    /// @param message エラーメッセージ
    /// @param funcName 呼び出し元の関数名
    /// @param fileName 呼び出し元のファイル名
    /// @param lineNumber 呼び出し元の行番号
    inline void PrintAppErrorMessage(
        const std::string& message,
        const char* funcName,
        const char* fileName,
        int lineNumber)
    {
        PrintAppErrorInfo(message, funcName, fileName, lineNumber,
            DebugPrintConfig::GetInstance().GetPrintErrorMessageColor());

        if (DebugPrintConfig::GetInstance().IsExitOnError())
        {
            std::exit(EXIT_FAILURE);
        }
    }

    /// @brief POPUP_MESSAGE マクロから呼び出されるポップアップ表示処理。
    /// Config の POPUP_MESSAGE 用の色でコンソールに表示し、ポップアップを表示する。アプリは継続する
    /// @param message 表示するメッセージ
    /// @param icon アイコン種別
    inline void ShowPopupMessage(const std::string& message, PopupIcon icon)
    {
        PrintMessage(message, DebugPrintConfig::GetInstance().GetPopupMessageColor());
        ShowPopup(message, icon);
    }

    /// @brief POPUP_WARNING_MESSAGE マクロから呼び出される警告ポップアップ表示処理。
    /// Config の POPUP_WARNING_MESSAGE 用の色でコンソールに表示し、警告アイコン付きポップアップを表示する。アプリは継続する
    /// @param message 警告メッセージ
    /// @param funcName 呼び出し元の関数名
    /// @param fileName 呼び出し元のファイル名
    /// @param lineNumber 呼び出し元の行番号
    inline void ShowPopupWarningMessage(
        const std::string& message,
        const char* funcName,
        const char* fileName,
        int lineNumber)
    {
        PrintAppErrorInfo(message, funcName, fileName, lineNumber,
            DebugPrintConfig::GetInstance().GetPopupWarningMessageColor());
        ShowPopup(message, PopupIcon::Warning);
    }

    /// @brief POPUP_ERROR_MESSAGE マクロから呼び出されるエラーポップアップ表示処理。
    /// Config の POPUP_ERROR_MESSAGE 用の色でコンソールに表示し、エラーアイコン付きポップアップを表示する。
    /// IsExitOnError() が true の場合はアプリを終了する
    /// @param message エラーメッセージ
    /// @param funcName 呼び出し元の関数名
    /// @param fileName 呼び出し元のファイル名
    /// @param lineNumber 呼び出し元の行番号
    inline void ShowPopupErrorMessage(
        const std::string& message,
        const char* funcName,
        const char* fileName,
        int lineNumber)
    {
        PrintAppErrorInfo(message, funcName, fileName, lineNumber,
            DebugPrintConfig::GetInstance().GetPopupErrorMessageColor());
        ShowPopup(message, PopupIcon::Error);

        if (DebugPrintConfig::GetInstance().IsExitOnError())
        {
            std::exit(EXIT_FAILURE);
        }
    }

} // namespace DebugPrint
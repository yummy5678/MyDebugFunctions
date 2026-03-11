#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>


#ifndef __EMSCRIPTEN__
    #ifdef DEBUG_PRINT_IMPLEMENTATION
    /* ファイルの改変あり
        666行目・724行目のchar*およびwchar_t*にconstを追加。
        C++コンパイラでのコンパイルに対応するための修正。 */
    #include "../third_party/tinyfiledialogs/tinyfiledialogs.c"
    #endif
    #include "../third_party/tinyfiledialogs/tinyfiledialogs.h"
#else
    // Emscripten 環境用の JavaScript 関数定義を読み込む
    #include "EmscriptenFunctions.h"
#endif



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
#if defined(__EMSCRIPTEN__)

        if (DebugPrintConfig::GetInstance().IsColorOutputEnabled())
        {
            js_print(message.c_str(), color);
        }
        else
        {
            js_print(message.c_str(), PRINT_COLOR::DEFAULT);
        }

#else
        if (DebugPrintConfig::GetInstance().IsColorOutputEnabled())
        {
            std::cout << MakeColorCode(color) << message << MakeColorCode(PRINT_COLOR::DEFAULT) << std::flush;
        }
        else
        {
            std::cout << message << std::flush;
        }
#endif
    }

    /// @brief メッセージを標準エラー出力に表示する基本関数。
    /// 指定色でメッセージを表示する
    /// @param message 表示するメッセージ
    /// @param color 表示色
    inline void PrintErrorMessage(const std::string& message, Color color = PRINT_COLOR::DEFAULT)
    {
#if defined(__EMSCRIPTEN__)

        if (DebugPrintConfig::GetInstance().IsColorOutputEnabled())
        {
            js_print_error(message.c_str(), color);
        }
        else
        {
            js_print_error(message.c_str(), PRINT_COLOR::DEFAULT);
        }


#else
        if (DebugPrintConfig::GetInstance().IsColorOutputEnabled())
        {
            std::cerr << MakeColorCode(color) << message << MakeColorCode(PRINT_COLOR::DEFAULT) << std::flush;
        }
        else
        {
            std::cerr << message << std::flush;
        }
#endif
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
        PrintErrorMessage(out.str(), color);
    }

    /// @brief ポップアップダイアログを表示する内部共通処理。
    /// Emscripten ではブラウザの SweetAlert2 を使用したモーダルダイアログを表示し、
    /// それ以外は tinyfiledialogs を使用する。
    /// ファイル名・関数名が渡された場合はダイアログ内の詳細情報欄にも表示する。
    /// SweetAlert2 の Swal.fire() は Promise を返すため、C++ 側はダイアログの
    /// 閉じるのを待たずに即リターンする（-sASYNCIFY 不要）。
    /// Node.js 環境では SweetAlert2 が使えないため stderr に出力してフォールバックする
    /// @param message 表示するメッセージ
    /// @param icon アイコン種別
    /// @param funcName 呼び出し元の関数名。nullptr の場合は詳細情報を表示しない
    /// @param fileName 呼び出し元のファイル名。nullptr の場合は詳細情報を表示しない
    /// @param lineNumber 呼び出し元の行番号
    inline void ShowPopup(
        const std::string& message,
        PopupIcon icon,
        const char* funcName  = nullptr,
        const char* fileName  = nullptr,
        int         lineNumber = 0)
    {
#if defined(__EMSCRIPTEN__)
        // ファイル名・行番号・関数名の情報を HTML 形式で組み立てる。
        // ファイル名または関数名が渡されていない場合は詳細情報を表示しない
        std::ostringstream detail;
        if (fileName != nullptr && funcName != nullptr)
        {
            detail << "<hr style='margin:8px 0'>"
                   << "<div style='text-align:left; font-size:0.85em; color:#666'>"
                   << fileString()         << pairSeparatorString() << fileName   << "<br>"
                   << LineNumberString()   << pairSeparatorString() << lineNumber << "<br>"
                   << functionNameString() << pairSeparatorString() << funcName
                   << "</div>";
        }
        const std::string htmlDetail = detail.str();


        js_show_popup(message.c_str(), PopupIconToString(icon), htmlDetail.c_str());


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
    /// Config の POPUP_MESSAGE 用の色でコンソールに表示し、ポップアップを表示する。
    /// ファイル名・行番号などの詳細情報は表示しない。アプリは継続する
    /// @param message 表示するメッセージ
    /// @param icon アイコン種別
    inline void ShowPopupMessage(const std::string& message, PopupIcon icon)
    {
        ShowPopup(message, icon);
    }

    /// @brief POPUP_WARNING_MESSAGE マクロから呼び出される警告ポップアップ表示処理。
    /// Config の POPUP_WARNING_MESSAGE 用の色でコンソールに表示し、
    /// 警告アイコン付きポップアップにファイル名・行番号・関数名も含めて表示する。アプリは継続する
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
        ShowPopup(message, PopupIcon::Warning, funcName, fileName, lineNumber);
    }

    /// @brief POPUP_ERROR_MESSAGE マクロから呼び出されるエラーポップアップ表示処理。
    /// Config の POPUP_ERROR_MESSAGE 用の色でコンソールに表示し、
    /// エラーアイコン付きポップアップにファイル名・行番号・関数名も含めて表示する。
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
        ShowPopup(message, PopupIcon::Error, funcName, fileName, lineNumber);

        // エラー時の強制終了フラグが立っていたら終了させる
        if (DebugPrintConfig::GetInstance().IsExitOnError())
        {
            std::exit(EXIT_FAILURE);
        }
    }

} // namespace DebugPrint
#pragma once

// 変数名を文字列として取得するマクロ
#define THIS_PARAM_NAME(param) #param

// ファイル名を文字列として取得するマクロ
#define THIS_FILE_NAME __FILE__

// ここの行番号を取得するマクロ
#define THIS_LINE_NUMBER __LINE__

// 関数名を文字列として取得するマクロ
#if defined(__GNUC__) || defined(__clang__)
        // GCC または Clang の場合
        // __PRETTY_FUNCTION__ は関数の詳細なシグネチャ（名前空間、クラス名、引数、const などを含む）を返します。
#define THIS_FUNCTION_NAME __PRETTY_FUNCTION__

#elif defined(_MSC_VER)
        // Microsoft Visual C++ (MSVC) の場合
        // __FUNCSIG__ は関数の詳細なシグネチャ(呼び出し規約なども含む場合がある)を返します。
#define THIS_FUNCTION_NAME __FUNCSIG__

#else
        // それ以外のコンパイラの場合
        // 標準C++の __func__ を使います。
        // __func__ は関数名のみを返し、シグネチャは含まれません。
#define THIS_FUNCTION_NAME __func__
#endif


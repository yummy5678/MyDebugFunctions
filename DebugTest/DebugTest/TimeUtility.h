#pragma once
#include <iostream>      // 標準入出力を使うため
#include <chrono>        // 時間取得用
#include <ctime>         // 時間処理用
#include <iomanip>       // 出力フォーマット用
#include <sstream>       // 文字列ストリーム用



namespace DebugPrint
{
    constexpr const char* defaultTimeFormat = "%Y-%m-%d %H:%M:%S%z";   // 時間の表現方法


    // 時間取得用関数
    std::string GetDateTimeString(const char* timeFormat = defaultTimeFormat);


    class Timer
    {
    public:
        // 時間の計測をスタート
        void Start();

        // スタートしてからの現在の経過時間を文字列で取得
        std::string GetElapsedSecTime();

    private:
        std::chrono::system_clock::time_point start;
    };
}


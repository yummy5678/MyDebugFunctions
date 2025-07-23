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
    inline std::string GetDateTimeString(const char* timeFormat = defaultTimeFormat)
    {
        // 現在時刻をシステムクロックから取得
        auto now = std::chrono::system_clock::now();

        // time_t に変換(C言語形式の時刻型)
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // ローカル時刻に変換(tm 構造体に変換)
        std::tm local_tm{};
#ifdef _WIN32
        localtime_s(&local_tm, &now_c);  // Windows用関数
#else
        localtime_r(&now_c, &local_tm);  // Linux/Unix用関数
#endif

        // 出力用の文字列バッファ
        char buffer[64];

        // 時刻を指定フォーマットで文字列に変換
        std::strftime(buffer, sizeof(buffer), timeFormat, &local_tm);

        return buffer;
    }


    class Timer
    {
    public:
        // 時間の計測をスタート
        void Start()
        {
            start = std::chrono::system_clock::now();
        }

        // スタートしてからの現在の経過時間を文字列で取得
        std::string GetElapsedSecTime()
        {
            auto now = std::chrono::system_clock::now();
            auto duration = now - start;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            auto sec = ms / 1000;
            auto msec = ms % 1000;

            std::ostringstream oss;
            oss << sec << "." << std::setw(3) << std::setfill('0') << msec;
            return oss.str();
        }

    private:
        std::chrono::system_clock::time_point start;
    };
}


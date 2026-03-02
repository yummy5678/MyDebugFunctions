#pragma once
#include <chrono>        // 時間取得用
#include <ctime>         // 時間処理用
#include <iomanip>       // 出力フォーマット用
#include <sstream>       // 文字列ストリーム用
#include <string>
#include "TemplateStrings.h"

namespace DebugPrint
{
    /// @brief 現在時刻を指定フォーマットの文字列で取得する。
    /// timeFormat が nullptr の場合は TemplateStrings から日時フォーマットを取得する。
    /// Windows では localtime_s、それ以外では localtime_r を使用する
    /// @param timeFormat 日時フォーマット文字列。nullptr の場合は設定値を使用する
    /// @return フォーマットされた日時文字列
    [[nodiscard]] inline std::string GetDateTimeString(const char* timeFormat = nullptr)
    {
        // timeFormat が指定されていない場合は設定から取得する
        if (timeFormat == nullptr)
        {
            timeFormat = TemplateStrings::GetInstance().Get(keyDatetimeFormat).c_str();
        }

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


    /// @brief 経過時間を計測するクラス。
    /// Start() で計測を開始し、GetElapsedSecTime() で経過時間を秒単位の文字列で取得する。
    /// FunctionTracer から関数の実行時間計測に使用する
    class Timer
    {
    public:
        /// @brief 経過時間の計測を開始する
        void Start()
        {
            m_Start = std::chrono::system_clock::now();
        }

        /// @brief Start() からの経過時間を "秒.ミリ秒" 形式の文字列で取得する
        /// @return 経過時間の文字列 ("1.234" など)
        [[nodiscard]] std::string GetElapsedSecTime()
        {
            auto now      = std::chrono::system_clock::now();
            auto duration = now - m_Start;
            auto ms       = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            auto sec      = ms / 1000;
            auto msec     = ms % 1000;

            std::ostringstream oss;
            oss << sec << "." << std::setw(3) << std::setfill('0') << msec;
            return oss.str();
        }

    private:
        std::chrono::system_clock::time_point m_Start;  // 計測開始時刻
    };

} // namespace DebugPrint
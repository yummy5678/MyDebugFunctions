#include "DebugUtility.h"

std::string DebugUtility::GetDateTimeString(const char* timeFormat)
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

void DebugUtility::Timer::Start()
{
    start = std::chrono::system_clock::now();
}

std::string DebugUtility::Timer::GetElapsedSecTime()
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


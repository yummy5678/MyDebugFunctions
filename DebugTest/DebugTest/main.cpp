#include <cstdlib>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include "DebugPrint.h"

// クラストレースのテスト用クラス
class HogeClass
{
public:
    HogeClass() = default;
    ~HogeClass() = default;

private:
    PRINT_TRACE_CLASS(HogeClass);
};

// magic_enum のテスト用列挙型
enum class TestEnum
{
    VALUE_A,
    VALUE_B,
    VALUE_C,
};

// 通常の構造体
struct Vec3
{
    float x;
    float y;
    float z;
};

// 列挙型メンバーを持つ構造体
enum class Direction { North, South, East, West };

struct Player
{
    int       id;
    float     hp;
    Direction dir;
};

// ネストした構造体
struct Transform
{
    Vec3  position;
    Vec3  scale;
    float rotation;
};

int main(int argc, char* argv[])
{
    PRINT_TRACE_FUNCTION;

    // ===== Config の設定 =====
    DEBUG_SET_LOG_PATH("./logs/");
    DEBUG_SET_EXIT_ON_ERROR(false);
    DEBUG_SET_PRINT_MESSAGE_COLOR(PRINT_COLOR::DEFAULT);
    DEBUG_SET_PRINT_WARNING_MESSAGE_COLOR(PRINT_COLOR::YELLOW);
    DEBUG_SET_PRINT_ERROR_MESSAGE_COLOR(PRINT_COLOR::BRIGHT_RED);
    DEBUG_SET_POPUP_MESSAGE_COLOR(PRINT_COLOR::DEFAULT);
    DEBUG_SET_POPUP_WARNING_MESSAGE_COLOR(PRINT_COLOR::YELLOW);
    DEBUG_SET_POPUP_ERROR_MESSAGE_COLOR(PRINT_COLOR::BRIGHT_RED);

    // ===== クラストレースのテスト =====
    HogeClass hoge;

    // ===== 通常メッセージのテスト =====
    PRINT_MESSAGE("PRINT_MESSAGE: 通常メッセージ\n");

    // ===== 変数表示のテスト =====
    int                      a = 10;
    int                      arr[] = { 1, 2, 3 };
    std::vector<std::string> nameList = { "Alice", "Bob" };
    std::array<float, 3>     ary = { 3.14f, 2.71f, 1.61f };
    TestEnum                 enumVal = TestEnum::VALUE_B;

    PRINT_VARIABLE(a);
    PRINT_VARIABLE(arr);
    PRINT_VARIABLE(nameList);
    PRINT_VARIABLE(ary);
    PRINT_VARIABLE(enumVal);  // magic_enum により "VALUE_B" と表示される


    Vec3      vec = { 1.0f, 2.0f, 3.0f };
    Player    player = { 1, 100.0f, Direction::North };
    Transform transform = { { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 45.0f };

    PRINT_STRUCT(vec);
    PRINT_STRUCT(player);     // 列挙型メンバーも文字列で表示される
    PRINT_STRUCT(transform);  // ネストした Vec3 も再帰的に表示される


    // ===== 警告・エラーメッセージのテスト =====
    PRINT_WARNING_MESSAGE("PRINT_WARNING_MESSAGE: 警告メッセージ\n");
    PRINT_ERROR_MESSAGE("PRINT_ERROR_MESSAGE: エラーメッセージ\n");

    // ===== ポップアップのテスト =====
    POPUP_MESSAGE("POPUP_MESSAGE: 通常ポップアップ");
    POPUP_MESSAGE_ICON("POPUP_MESSAGE_ICON: 情報アイコン付きポップアップ", DebugPrint::PopupIcon::Info);
    POPUP_WARNING_MESSAGE("POPUP_WARNING_MESSAGE: 警告ポップアップ\n");
    POPUP_ERROR_MESSAGE("POPUP_ERROR_MESSAGE: エラーポップアップ\n");

    // ===== ログのテスト =====
    DebugPrint::LogWriter::GetInstance().Add("[INFO]    アプリケーション開始");
    DebugPrint::LogWriter::GetInstance().Add("[WARNING] テスト用警告メッセージ");
    DebugPrint::LogWriter::GetInstance().Add("[ERROR]   テスト用エラーメッセージ");

    PRINT_MESSAGE("\n--- ログエントリ一覧 ---\n");
    for (const auto& entry : DebugPrint::DebugPrintConfig::GetInstance().GetLogStrings())
    {
        PRINT_MESSAGE(entry + "\n");
    }
    PRINT_MESSAGE("------------------------\n\n");

    if (DEBUG_WRITE_LOG("./logs/"))
    {
        PRINT_MESSAGE("ログファイルの書き出しに成功しました\n");
    }
    else
    {
        PRINT_MESSAGE("ログファイルの書き出しに失敗しました\n");
    }

    return EXIT_SUCCESS;
}
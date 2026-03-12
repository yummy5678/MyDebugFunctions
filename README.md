# DebugPrint

C++20 ヘッダーオンリーのデバッグ出力ライブラリ。

## デモ

WebAssembly版をブラウザで試せます：
**[オンラインデモ](https://yummy5678.github.io/MyDebugFunctions/)**

## 機能

- メッセージ出力（通常・警告・エラー）
- 変数・構造体の自動表示（Boost.PFR / magic_enum）
- 関数・クラスのトレース
- ポップアップダイアログ（ネイティブ: tinyfiledialogs / Wasm: SweetAlert2）
- カラー出力対応
- ログファイル書き出し
- 多言語対応（日本語・英語）

## 使用例
```cpp
#define DEBUG_PRINT_IMPLEMENTATION
#include "DebugPrint/DebugPrint.h"

int main()
{
    // メッセージ出力
    PRINT_MESSAGE("通常メッセージ\n");
    PRINT_WARNING_MESSAGE("警告\n");
    PRINT_ERROR_MESSAGE("エラー\n");

    // 変数・構造体の自動表示
    int value = 42;
    PRINT_VARIABLE(value);    // "value = 42"

    struct Vec3 { float x, y, z; };
    Vec3 pos = { 1.0f, 2.0f, 3.0f };
    PRINT_STRUCT(pos);        // メンバーを自動展開

    // ポップアップダイアログ
    POPUP_MESSAGE("ポップアップ表示");

    // 関数・クラスのトレース
    PRINT_TRACE_FUNCTION;
}
```
## サードパーティライブラリ

- [Boost.PFR](https://github.com/boostorg/pfr) - Boost Software License 1.0
- [magic_enum](https://github.com/Neargye/magic_enum) - MIT License
- [RapidJSON](https://github.com/Tencent/rapidjson) - MIT License
- [SweetAlert2](https://github.com/sweetalert2/sweetalert2) - MIT License
- [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) - zlib License

## ライセンス

MIT License

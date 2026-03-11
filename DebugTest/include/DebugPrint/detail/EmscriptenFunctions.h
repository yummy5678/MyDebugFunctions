#pragma once
#include "ColorDefine.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>

/// @brief Emscripten 環境で使用する JavaScript 関数の定義をまとめたファイル。
/// EM_JS マクロは名前空間の外側でのみ定義できるため、このファイルで一元管理する。
/// インクルードは PrintFunction.h からのみ行う


// カラー出力が可能な環境かどうかを判定する。
// Node.jsのTTY環境またはブラウザ環境であれば1を返す
EM_JS(int, js_is_color_output_available, (), {
    var isNode = typeof process !== 'undefined' &&
                    process.stdout !== undefined &&
                    process.stdout.isTTY ? 1 : 0;
    var isBrowser = typeof window !== 'undefined' ? 1 : 0;
    return (isNode || isBrowser) ? 1 : 0;
});


// カラー変換ヘルパーと SweetAlert2 のグローバル登録をまとめて行う。
// DebugPrintConfig のコンストラクタで一度だけ呼び出す
EM_JS(void, js_register_helpers, (), {
    // Color型（RGBA順のuint32_t）をCSSのrgb文字列に変換する関数を登録する
    globalThis.makeColorCode = function(color) {
        var a = (color >>> 0) & 0xFF;
        if (a === 0) return null;
        var ratio = a / 255.0;
        var r = Math.round(((color >>> 24) & 0xFF) * ratio);
        var g = Math.round(((color >>> 16) & 0xFF) * ratio);
        var b = Math.round(((color >>>  8) & 0xFF) * ratio);
        return 'rgb(' + r + ',' + g + ',' + b + ')';
    };
    // sweetalert2 が --pre-js でモジュールスコープに読み込まれるため
    // globalThis.Swal として登録する
    globalThis.Swal = globalThis.Sweetalert2;
});


// SweetAlert2 のダイアログが閉じられるまで C++ 側の実行を待機する。
// EM_ASYNC_JS を使うことで Swal.fire() の Promise が解決するまでブロックする。
// ブラウザ環境以外では stderr にフォールバックする
EM_ASYNC_JS(void, js_show_popup, (const char* msg, const char* iconCStr, const char* detailCStr), {
    if (typeof window === 'undefined' || typeof Swal === 'undefined') {
        var text = UTF8ToString(msg) + UTF8ToString(detailCStr);
        process.stderr.write('[POPUP] ' + text);
        return;
    }

    var iconStr = UTF8ToString(iconCStr);
    var titleStr = iconStr === 'info' ? '情報'
                 : iconStr === 'warning' ? '警告'
                 : iconStr === 'error' ? 'エラー'
                 : iconStr === 'question' ? '確認'
                 : 'メッセージ';
    var hasIcon = iconStr.length > 0;

    // Swal.fire() の Promise を直接返すことで C++ 側がダイアログを閉じるまで待機する
    return Swal.fire({
        title:             titleStr,
        html : UTF8ToString(msg) + UTF8ToString(detailCStr),
        icon : hasIcon ? iconStr : undefined,
        confirmButtonText : 'OK'
    });
});



EM_JS(void, js_print, (const char* msg, Color color), {
    var text = UTF8ToString(msg);
    var output = document.getElementById('output');
    if (!output) { console.log(text); return; }
    var span = document.createElement('span');
    var cssColor = globalThis.makeColorCode(color);
    if (cssColor) span.style.color = cssColor;
    span.textContent = text;
    output.appendChild(span);
    });


EM_JS(void, js_print_error, (const char* msg, Color color), {
    var text = UTF8ToString(msg);
    var output = document.getElementById('output');
    if (!output) { console.error(text); return; }
    var span = document.createElement('span');
    var cssColor = globalThis.makeColorCode(color);
    if (cssColor) span.style.color = cssColor;
    span.textContent = text;
    output.appendChild(span);
    });


#endif
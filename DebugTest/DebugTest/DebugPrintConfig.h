#pragma once
#include <string>
#include <vector>

#if defined(__EMSCRIPTEN__)
#elif defined(_WIN32)
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <cstdlib>
#endif

#include "ColorDefine.h"
#include "TemplateStrings.h"
#include "LogWriter.h"

namespace DebugPrint
{
    /// @brief デバッグ出力ライブラリのユーザー向け設定を管理するシングルトンクラス。
    /// カラー出力・ログファイル出力の有効・無効、言語切り替えの指示を担う。
    /// 各マクロの表示色・エラー時の終了有無もここで設定する。
    /// カラー出力が端末で使えるかの判定も管理する。
    /// 文字列管理は TemplateStrings、ログ蓄積は LogWriter に委譲する
    class DebugPrintConfig
    {
    public:

        /// @brief シングルトンのインスタンスを取得する
        static DebugPrintConfig& GetInstance()
        {
            static DebugPrintConfig instance;
            return instance;
        }

        /// @brief 言語を切り替える。TemplateStrings に委譲して文字列を再読み込みする
        /// @param lang 言語コード ("ja" または "en")
        /// @param languagePath 言語ファイルのディレクトリパス
        void SetLanguage(const std::string& lang, const std::string& languagePath = "./lang/")
        {
            TemplateStrings::GetInstance().SetLanguage(lang, languagePath);
        }

        /// @brief 日時フォーマットを変更する。TemplateStrings に委譲する
        /// @param format 日時フォーマット文字列 ("%Y-%m-%d %H:%M:%S" など)
        void SetDatetimeFormat(const std::string& format)
        {
            TemplateStrings::GetInstance().Set(keyDatetimeFormat, format);
        }

        /// @brief ログファイルの出力先ディレクトリパスを設定する
        /// @param path ログファイルの出力先パス ("./logs/" など)
        void SetLogPath(const std::string& path) { m_LogPath = path; }

        /// @brief ログファイルの出力先ディレクトリパスを取得する
        const std::string& GetLogPath() const { return m_LogPath; }

        /// @brief エラー系マクロ呼び出し時にアプリを終了するかどうかを設定する
        /// @param enabled true で終了する、false で継続する
        void SetExitOnError(bool enabled) { m_ExitOnError = enabled; }

        /// @brief エラー系マクロ呼び出し時に終了するかどうかを取得する
        bool IsExitOnError() const { return m_ExitOnError; }

        // --- 各マクロの表示色設定 ---

        /// @brief PRINT_MESSAGE の表示色を設定する
        void SetPrintMessageColor(Color color) { m_PrintMessageColor = color; }
        /// @brief PRINT_WARNING_MESSAGE の表示色を設定する
        void SetPrintWarningMessageColor(Color color) { m_PrintWarningMessageColor = color; }
        /// @brief PRINT_ERROR_MESSAGE の表示色を設定する
        void SetPrintErrorMessageColor(Color color) { m_PrintErrorMessageColor = color; }
        /// @brief POPUP_MESSAGE の表示色を設定する
        void SetPopupMessageColor(Color color) { m_PopupMessageColor = color; }
        /// @brief POPUP_WARNING_MESSAGE の表示色を設定する
        void SetPopupWarningMessageColor(Color color) { m_PopupWarningMessageColor = color; }
        /// @brief POPUP_ERROR_MESSAGE の表示色を設定する
        void SetPopupErrorMessageColor(Color color) { m_PopupErrorMessageColor = color; }

        /// @brief カラー出力が有効かどうかを取得する
        bool  IsColorOutputEnabled()        const { return m_ColorOutputAvailable; }
        /// @brief PRINT_MESSAGE の表示色を取得する
        Color GetPrintMessageColor()        const { return m_PrintMessageColor; }
        /// @brief PRINT_WARNING_MESSAGE の表示色を取得する
        Color GetPrintWarningMessageColor() const { return m_PrintWarningMessageColor; }
        /// @brief PRINT_ERROR_MESSAGE の表示色を取得する
        Color GetPrintErrorMessageColor()   const { return m_PrintErrorMessageColor; }
        /// @brief POPUP_MESSAGE の表示色を取得する
        Color GetPopupMessageColor()        const { return m_PopupMessageColor; }
        /// @brief POPUP_WARNING_MESSAGE の表示色を取得する
        Color GetPopupWarningMessageColor() const { return m_PopupWarningMessageColor; }
        /// @brief POPUP_ERROR_MESSAGE の表示色を取得する
        Color GetPopupErrorMessageColor()   const { return m_PopupErrorMessageColor; }

        /// @brief 蓄積されたログエントリを文字列のリストで取得する。LogWriter に委譲する
        /// @return ログエントリの文字列リスト
        const std::vector<std::string>& GetLogStrings() const
        {
            return LogWriter::GetInstance().GetEntries();
        }

        /// @brief 蓄積されたログエントリをファイルに書き出す。LogWriter に委譲する
        /// @return 書き出しに成功した場合は true、失敗した場合は false
        bool WriteLog()
        {
            return LogWriter::GetInstance().WriteToFile(m_LogPath);
        }

        // コピー・ムーブ禁止
        DebugPrintConfig(const DebugPrintConfig&) = delete;
        DebugPrintConfig& operator=(const DebugPrintConfig&) = delete;
        DebugPrintConfig(DebugPrintConfig&&) = delete;
        DebugPrintConfig& operator=(DebugPrintConfig&&) = delete;

    private:

        /// @brief コンストラクタ。起動時に端末のカラー出力対応状況を確認する
        DebugPrintConfig()
            : m_ColorOutputAvailable(CheckColorOutputAvailable())
        {
        }

        /// @brief 端末がカラー出力(ANSIエスケープコード)に対応しているか確認する。
        /// Windows では仮想ターミナル処理が有効かどうかを確認する。
        /// Unix 系では TERM 環境変数を確認する。
        /// 起動時に1度だけ呼び出され、結果を m_ColorOutputAvailable に保持する
        /// @return カラー出力が使える場合は true
        static bool CheckColorOutputAvailable()
        {
#if defined(__EMSCRIPTEN__)
            return false;
#elif defined(_WIN32)
            if (!_isatty(_fileno(stdout))) return false;

            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut == INVALID_HANDLE_VALUE) return false;

            DWORD dwMode = 0;
            if (!GetConsoleMode(hOut, &dwMode)) return false;

            // 仮想ターミナル処理(ANSIエスケープコード)が有効かどうか確認する
            DWORD newMode = dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if (!SetConsoleMode(hOut, newMode)) return false;

            SetConsoleMode(hOut, dwMode);  // 副作用防止のため元に戻す
            return true;
#else
            if (!isatty(STDOUT_FILENO)) return false;

            const char* term = std::getenv("TERM");
            if (term == nullptr) return false;

            return std::string(term) != "dumb";
#endif
        }

        std::string m_LogPath = "./logs/";  // ログファイルの出力先パス
        bool        m_ColorOutputAvailable; // 端末がカラー出力に対応しているかどうか
        bool        m_ExitOnError = false;  // エラー系マクロ呼び出し時の終了有無

        // 各マクロの表示色
        Color m_PrintMessageColor           = PRINT_COLOR::DEFAULT;    // PRINT_MESSAGE の表示色
        Color m_PrintWarningMessageColor    = PRINT_COLOR::YELLOW;     // PRINT_WARNING_MESSAGE の表示色
        Color m_PrintErrorMessageColor      = PRINT_COLOR::BRIGHT_RED; // PRINT_ERROR_MESSAGE の表示色
        Color m_PopupMessageColor           = PRINT_COLOR::DEFAULT;    // POPUP_MESSAGE の表示色
        Color m_PopupWarningMessageColor    = PRINT_COLOR::YELLOW;     // POPUP_WARNING_MESSAGE の表示色
        Color m_PopupErrorMessageColor      = PRINT_COLOR::BRIGHT_RED; // POPUP_ERROR_MESSAGE の表示色
    };

} // namespace DebugPrint
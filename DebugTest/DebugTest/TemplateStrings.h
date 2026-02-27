#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

namespace DebugPrint
{
    /// @brief デバッグ出力ライブラリで使用する文字列を一元管理するシングルトンクラス。
    /// 言語ファイル(JSON)の読み込みと文字列の提供を担う。
    /// 言語の切り替えは SetLanguage() で行い、対応するJSONファイルを再読み込みする。
    /// JSONファイルが見つからない・解析失敗時は日本語のフォールバック文字列を使用する
    class TemplateStrings
    {
    public:

        /// @brief シングルトンのインスタンスを取得する
        static TemplateStrings& GetInstance()
        {
            static TemplateStrings instance;
            return instance;
        }

        /// @brief 言語を切り替えて言語ファイルを再読み込みする
        /// @param lang 言語コード ("ja" または "en")
        /// @param languagePath 言語ファイルのディレクトリパス
        void SetLanguage(const std::string& lang, const std::string& languagePath = "./lang/")
        {
            m_Language = lang;
            m_LanguagePath = languagePath;
            LoadLanguageFile();
        }

        /// @brief キーに対応する文字列を変更する。
        /// 存在しないキーを指定した場合は何もしない
        /// @param key 文字列キー
        /// @param value 変更後の文字列
        void Set(const std::string& key, const std::string& value)
        {
            auto it = m_Strings.find(key);
            if (it == m_Strings.end())
            {
                return;
            }
            it->second = value;
        }

        /// @brief キーに対応する文字列を取得する。
        /// キーが存在しない場合はキー名をそのまま返す
        /// @param key 文字列キー
        const std::string& Get(const std::string& key) const
        {
            auto it = m_Strings.find(key);
            if (it != m_Strings.end())
            {
                return it->second;
            }
            return key;
        }

        // コピー・ムーブ禁止
        TemplateStrings(const TemplateStrings&) = delete;
        TemplateStrings& operator=(const TemplateStrings&) = delete;
        TemplateStrings(TemplateStrings&&) = delete;
        TemplateStrings& operator=(TemplateStrings&&) = delete;

    private:

        /// @brief コンストラクタ。デフォルト設定(日本語)で初期化し言語ファイルを読み込む
        TemplateStrings()
        {
            LoadLanguageFile();
        }

        /// @brief 言語ファイル(JSON)を読み込み、文字列テーブルを構築する。
        /// ファイルが見つからない・解析失敗時は日本語フォールバックを使用する
        void LoadLanguageFile()
        {
            m_Strings.clear();

            const std::string filePath = m_LanguagePath + m_Language + ".json";

            std::ifstream file(filePath);
            if (!file.is_open())
            {
                LoadFallbackStrings();
                return;
            }

            std::ostringstream buffer;
            buffer << file.rdbuf();
            const std::string content = buffer.str();

            rapidjson::Document doc;
            doc.Parse(content.c_str());

            if (doc.HasParseError() || !doc.IsObject())
            {
                LoadFallbackStrings();
                return;
            }

            for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
            {
                if (it->value.IsString())
                {
                    m_Strings[it->name.GetString()] = it->value.GetString();
                }
            }
        }

        /// @brief JSONファイルが見つからない・解析失敗時に使用する
        /// 日本語のフォールバック文字列をハードコードで設定する
        void LoadFallbackStrings()
        {
            m_Strings["datetimeFormat"] = "%Y-%m-%d %H:%M:%S%z";
            m_Strings["pairSeparator"] = ": ";
            m_Strings["separator"] = "=================================\n";
            m_Strings["functionName"] = "関数名";
            m_Strings["startFunction"] = "関数を開始します\n";
            m_Strings["endFunction"] = "関数を終了します\n";
            m_Strings["startClass"] = "コンストラクタを開始します\n";
            m_Strings["endClass"] = "デストラクタを終了します\n";
            m_Strings["endTimer"] = "関数終了までの経過時間";
            m_Strings["seconds"] = "秒";
            m_Strings["file"] = "ファイル";
            m_Strings["value"] = "値 ";
            m_Strings["lineNumber"] = "行番号";
            m_Strings["variable"] = "変数";
            m_Strings["open"] = "[";
            m_Strings["close"] = "]";
            m_Strings["errorDialogTitle"] = "エラー";
        }

        std::string m_Language = "ja";       // 現在の言語コード
        std::string m_LanguagePath = "./lang/";  // 言語ファイルのディレクトリパス

        std::unordered_map<std::string, std::string> m_Strings;  // 文字列テーブル
    };

    // 文字列キーの定数
    inline const std::string keyDatetimeFormat = "datetimeFormat";
    inline const std::string keyPairSeparator = "pairSeparator";
    inline const std::string keySeparator = "separator";
    inline const std::string keyFunctionName = "functionName";
    inline const std::string keyStartFunction = "startFunction";
    inline const std::string keyEndFunction = "endFunction";
    inline const std::string keyStartClass = "startClass";
    inline const std::string keyEndClass = "endClass";
    inline const std::string keyEndTimer = "endTimer";
    inline const std::string keySeconds = "seconds";
    inline const std::string keyFile = "file";
    inline const std::string keyValue = "value";
    inline const std::string keyLineNumber = "lineNumber";
    inline const std::string keyVariable = "variable";
    inline const std::string keyOpen = "open";
    inline const std::string keyClose = "close";
    inline const std::string keyErrorDialogTitle = "errorDialogTitle";

    // 各文字列へのアクセス用インライン関数
    inline const std::string& datetimeFormat() { return TemplateStrings::GetInstance().Get(keyDatetimeFormat); }
    inline const std::string& pairSeparatorString() { return TemplateStrings::GetInstance().Get(keyPairSeparator); }
    inline const std::string& separatorString() { return TemplateStrings::GetInstance().Get(keySeparator); }
    inline const std::string& functionNameString() { return TemplateStrings::GetInstance().Get(keyFunctionName); }
    inline const std::string& startFunctionString() { return TemplateStrings::GetInstance().Get(keyStartFunction); }
    inline const std::string& endFunctionString() { return TemplateStrings::GetInstance().Get(keyEndFunction); }
    inline const std::string& startClassString() { return TemplateStrings::GetInstance().Get(keyStartClass); }
    inline const std::string& endClassString() { return TemplateStrings::GetInstance().Get(keyEndClass); }
    inline const std::string& endTimerString() { return TemplateStrings::GetInstance().Get(keyEndTimer); }
    inline const std::string& secondsString() { return TemplateStrings::GetInstance().Get(keySeconds); }
    inline const std::string& fileString() { return TemplateStrings::GetInstance().Get(keyFile); }
    inline const std::string& valueString() { return TemplateStrings::GetInstance().Get(keyValue); }
    inline const std::string& LineNumberString() { return TemplateStrings::GetInstance().Get(keyLineNumber); }
    inline const std::string& variableString() { return TemplateStrings::GetInstance().Get(keyVariable); }
    inline const std::string& openString() { return TemplateStrings::GetInstance().Get(keyOpen); }
    inline const std::string& closeString() { return TemplateStrings::GetInstance().Get(keyClose); }
    inline const std::string& errorDialogTitle() { return TemplateStrings::GetInstance().Get(keyErrorDialogTitle); }

} // namespace DebugPrint
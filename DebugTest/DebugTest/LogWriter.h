#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "TimeUtility.h"

namespace DebugPrint
{
    /// @brief ログエントリを蓄積し、ファイル書き出しや文字列取得を行うシングルトンクラス。
    /// ログエントリは Add() で追加し、WriteToFile() でファイルに書き出す。
    /// ファイル名は WriteToFile() 呼び出し時の日時で自動生成される。
    /// ログレベルや付加情報は呼び出し側がメッセージに含める形で使用する
    class LogWriter
    {
    public:

        /// @brief シングルトンのインスタンスを取得する
        static LogWriter& GetInstance()
        {
            static LogWriter instance;
            return instance;
        }

        /// @brief ログエントリを追加する
        /// @param message メッセージ本文
        void Add(const std::string& message)
        {
            m_Entries.push_back(message);
        }

        /// @brief 蓄積されたログエントリを文字列のリストで取得する
        /// @return ログエントリの文字列リスト
        const std::vector<std::string>& GetEntries() const
        {
            return m_Entries;
        }

        /// @brief 蓄積されたログエントリをファイルに書き出す。
        /// ファイル名は呼び出し時の日時で自動生成される。
        /// 出力先ディレクトリが存在しない場合は自動的に作成する。
        /// 書き出し後もエントリは保持される
        /// @param logPath ログファイルの出力先ディレクトリパス
        /// @return 書き出しに成功した場合は true、失敗した場合は false
        bool WriteToFile(const std::string& logPath)
        {
            // 出力先ディレクトリが存在しない場合は作成する
            std::filesystem::path dirPath(logPath);
            if (!std::filesystem::exists(dirPath))
            {
                std::filesystem::create_directories(dirPath);
            }

            // 呼び出し時の日時でファイル名を生成する
            const std::string filePath = logPath + GenerateFileName();
            std::ofstream file(filePath, std::ios::out | std::ios::trunc);
            if (!file.is_open())
            {
                return false;
            }

            // エントリを1行ずつ書き出す
            for (const auto& entry : m_Entries)
            {
                file << entry << "\n";
            }

            return true;
        }

        /// @brief 蓄積されたログエントリをすべて消去する
        void Clear()
        {
            m_Entries.clear();
        }

        /// @brief 蓄積されているエントリ数を取得する
        size_t GetEntryCount() const { return m_Entries.size(); }

        // コピー・ムーブ禁止
        LogWriter(const LogWriter&) = delete;
        LogWriter& operator=(const LogWriter&) = delete;
        LogWriter(LogWriter&&) = delete;
        LogWriter& operator=(LogWriter&&) = delete;

    private:

        LogWriter() = default;

        /// @brief ログファイル名を現在時刻から生成する。
        /// ファイル名の形式は "YYYY-MM-DD_HH-MM-SS.log" になる
        /// @return 生成されたファイル名
        std::string GenerateFileName() const
        {
            // ファイル名に使えない文字(:)を除いたフォーマットで生成する
            return GetDateTimeString("%Y-%m-%d_%H-%M-%S") + ".log";
        }

        std::vector<std::string> m_Entries;  // 蓄積されたログエントリ
    };

} // namespace DebugPrint
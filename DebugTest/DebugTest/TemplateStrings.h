#pragma once
#include <string>

// C++17～
namespace DebugPrint
{
	inline const std::string datetimeFormat			= "%Y-%m-%d %H:%M:%S%z";   // 時間の表現方法

	inline const std::string pairSeparatorString	= ": ";
	inline const std::string separatorString		= "=================================\n";
	inline const std::string startFunctionString	= "関数を開始します\n";
	inline const std::string endFunctionString		= "関数を終了します\n";
	inline const std::string startClassString		= "コンストラクタを開始します\n";
	inline const std::string endClassString			= "デストラクタを終了します\n";
	inline const std::string endTimerString			= "関数終了までの経過時間";
	inline const std::string secondsString			= "秒";
	inline const std::string fileString				= "ファイル";
	inline const std::string valueString			= "値 ";
	inline const std::string LineNumberString		= "行番号";

	inline const std::string variableString			= "変数";
	inline const std::string openString				= "[";
	inline const std::string closeString			= "]";

}


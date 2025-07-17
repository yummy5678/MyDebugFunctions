#pragma once
#include <string>

namespace DebugUtility
{
	static const std::string datetimeFormat			= "%Y-%m-%d %H:%M:%S%z";   // 時間の表現方法

	static const std::string separatorString		= "=================================\n";
	static const std::string startFunctionString	= "関数を開始します\n";
	static const std::string endFunctionString		= "関数を終了します\n";
	static const std::string endTimerString			= "関数終了までの経過時間 : ";
	static const std::string secondsString			= "秒";
	static const std::string FileString				= "ファイル : ";
	static const std::string LineNumberString		= "行番号 : ";
}


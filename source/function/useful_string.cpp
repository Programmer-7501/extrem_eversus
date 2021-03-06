
#include"../system/main.h"
#include"useful_string.h"
#include <sstream>

bool ExtractFileExpand(const std::string & fileName, std::string& out)
{
	// 後ろから'.'のある場所を探す
	int last_a_pos = static_cast<int>(fileName.find_last_of('.'));

	std::string substra;
	// '.'がないなら
	if (last_a_pos == std::string::npos) 
	{
		Logger::GetInstance().SetLog("ExtractFilePath 拡張子がありません");
		return false;
	}

	// '.'から後ろを切り取り
	out = fileName.substr(last_a_pos + 1);

	return true;
}

std::vector<std::string> Split(std::string & input, char delimiter)
{
	std::istringstream stream(input);
	std::string field;
	std::vector<std::string> result;
	while (getline(stream, field, delimiter)) 
	{
		result.push_back(field);
	}

	return result;
}

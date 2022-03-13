
#include"../system/main.h"
#include"useful_string.h"
#include <sstream>

bool ExtractFileExpand(const std::string & fileName, std::string& out)
{
	// Œã‚ë‚©‚ç'.'‚Ì‚ ‚éêŠ‚ğ’T‚·
	int last_a_pos = static_cast<int>(fileName.find_last_of('.'));

	std::string substra;
	// '.'‚ª‚È‚¢‚È‚ç
	if (last_a_pos == std::string::npos) 
	{
		Logger::GetInstance().SetLog("ExtractFilePath Šg’£q‚ª‚ ‚è‚Ü‚¹‚ñ");
		return false;
	}

	// '.'‚©‚çŒã‚ë‚ğØ‚èæ‚è
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

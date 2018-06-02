#include "colorMe_config.h"

namespace {
int ColorTextToInt(const std::string& code)
{
	if (code == "BLA") return 0;
	if (code == "RED") return 1;
	if (code == "GRE") return 2;
	if (code == "YEL") return 3;
	if (code == "BLU") return 4;
	if (code == "MAG") return 5;
	if (code == "CYA") return 6;
	if (code == "WHI") return 7;

	return 8;
}

int GetForw(const std::string& color)
{
	if (color.size() != 7)
		return 8;

	if (color[3] != '_')
		return 8;

	std::string fg = color.substr(0, 3);
	return ColorTextToInt(fg);
}

int GetBack(const std::string& color)
{
	if (color.size() != 7)
		return 8;

	if (color[3] != '_')
		return 8;

	std::string fg = color.substr(4, 3);
	return ColorTextToInt(fg);
}

std::string GetEscapeString(int fg, int bg)
{
	std::string out;
	if (fg > 8 || fg < 0)
		fg = 8;
	if (bg > 8 || bg < 0)
		bg = 8;

	//out = "\x1B[";
	if (fg != 8)
	{
		out += "3";
		out += std::to_string(fg);
	}


	if (bg != 8)
	{
		if (out.empty() == false)
			out += ";";

		out += "4";
		out += std::to_string(bg);
	}

	out += "m";

	return out;
}

}

namespace ColorMe {

void Config::ConfigureViaFile(const std::string & path, const std::string & filename)
{
	mConfigFile.open(path + "/" + filename, std::ios::in);
	if (mConfigFile.is_open() == false)
		return;

	ReadFile();

	mConfigFile.close();
}

Config::ColorStringToColorIndex Config::ReverseColorIndex()
{
	auto out = ColorStringToColorIndex();

	for (auto& kv : mColorIndexToColorString)
		out[kv.second] = kv.first;

	return out;
}

Config::ColorIndexToEscapeString Config::GenerateIndexToEscapeMap()
{
	auto out = ColorIndexToEscapeString();

	for (auto& kv : mColorIndexToColorString)
	{
		int8_t index = kv.first;
		const std::string& strColor = kv.second;

		std::string escape = GetEscapeString(GetForw(strColor), GetBack(strColor));
		out[index] = escape;
	}

	//
	// Add default color of index 0
	//

	std::string escape = GetEscapeString(8, 8);
	out[0] = escape;

	return out;
}

void Config::ReadFile()
{
	while (getline(mConfigFile, mCurrentLine))
	{
		if (IsCurrentLineColor() && IsValidColorString())
		{
			AddColorEntry();
			continue;
		}

		if (IsCurrentLinePattern())
		{
			AddPatternEntry();
			continue;
		}
	}
}

bool Config::IsCurrentLineColor()
{
	if (mCurrentLine.size() != 7)
		return false;

	if (mCurrentLine[3] != '_')
		return false;

	return true;
}

bool Config::IsValidColorString()
{
	return IsValidColorString(mCurrentLine);
}

bool Config::IsValidColorString(const std::string & text)
{
	if (text.size() != 7)
		return false;

	std::string fg = text.substr(0, 3);
	std::string bg = text.substr(4, 3);

	return IsValidColor(fg) && IsValidColor(bg);
}

bool Config::IsValidMatchTypeString(const std::string & text)
{
	if (text.size() != 1)
		return false;

	switch (text[0])
	{
	case 'F':
	case 'S':
	case 'f':
	case 's':
		return true;
	default:
		return false;
	}
}

bool Config::IsValidColor(const std::string test)
{
	if (test == "DEF") return true;
	if (test == "BLA") return true;
	if (test == "RED") return true;
	if (test == "GRE") return true;
	if (test == "YEL") return true;
	if (test == "BLU") return true;
	if (test == "MAG") return true;
	if (test == "CYA") return true;
	if (test == "WHI") return true;

	return false;
}

void Config::AddColorEntry()
{
	AddColorEntry(mCurrentLine);
}

void Config::AddColorEntry(const std::string & color)
{
	if (ColorAlreadyRegistered(color))
		return;

	int8_t index = mColorIndexToColorString.size() + 1;
	mColorIndexToColorString[index] = color;
}

bool Config::ColorAlreadyRegistered(const std::string & color)
{
	for (auto& kv : mColorIndexToColorString)
	{
		const std::string& storedColor = kv.second;
		if (storedColor == color)
			return true;
	}

	return false;
}

void Config::AddPatternEntry()
{
	std::string color = mRegexMatches[1];
	std::string matchType = mRegexMatches[2];
	std::string pattern = mRegexMatches[3];

	PatternData data;
	data.mColorString = color;

	switch (matchType[0])
	{
	case 'F':
	case 'f':
		data.mMatchType = MatchType::full_line;
		break;
	case 'S':
	case 's':
		data.mMatchType = MatchType::sub_line;
		break;
	}

	data.mPattern = pattern;

	mPatterns.push_back(data);
}

bool Config::IsCurrentLinePattern()
{
	std::regex_match(mCurrentLine, mRegexMatches, mRegexColorAndPattern);

	if (mRegexMatches.size() != 4)
		return false;

	std::string color = mRegexMatches[1];
	if (!IsValidColorString(color))
		return false;

	AddColorEntry(color);

	std::string matchType = mRegexMatches[2];
	if (!IsValidMatchTypeString(matchType))
		return false;
}

}

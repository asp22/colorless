#pragma once

#include<fstream>
#include<regex>
#include<map>
#include<vector>

namespace ColorMe {

enum MatchType
{
	full_line,
	sub_line,
};


class Config
{
public:
	struct PatternData
	{
		std::string mColorString;
		MatchType mMatchType;
		std::string mPattern;
	};

	using ColorIndexToColorString = std::map<int8_t, std::string>;
	ColorIndexToColorString mColorIndexToColorString;
	using ColorStringToColorIndex = std::map<std::string, int8_t>;
	using ColorIndexToEscapeString = ColorIndexToColorString;


	using ConfiguredPatterns = std::vector<PatternData>;
	ConfiguredPatterns mPatterns;

	void ConfigureViaFile(const std::string& path, const std::string& filename);

	ColorStringToColorIndex ReverseColorIndex();

	ColorIndexToEscapeString GenerateIndexToEscapeMap();

private:

	std::ifstream mConfigFile;
	std::string mCurrentLine;

	std::regex mRegexColorAndPattern{ R"(^(.*?)\s+(.*?)\s+(.*$))" };
	std::smatch mRegexMatches;

	void ReadFile();


	bool IsCurrentLineColor();

	bool IsValidColorString();

	bool IsValidColorString(const std::string& text);

	bool IsValidMatchTypeString(const std::string& text);

	bool IsValidColor(const std::string test);

	void AddColorEntry();

	void AddColorEntry(const std::string& color);

	bool ColorAlreadyRegistered(const std::string& color);

	void AddPatternEntry();

	bool IsCurrentLinePattern();
};

}


#include "regexColor.h"
#include "colorMe_interface.h"
#include "colorMe_config.h"

#include <memory>
#include <string>
#include <stdarg.h>

std::unique_ptr<ColorMe::RegexColor> sRegexColor{nullptr};
std::unique_ptr<ColorMe::Config> sConfig{nullptr};
std::unique_ptr<ColorMe::Config::ColorIndexToEscapeString> sIndexToEscape{nullptr};

void ColorMe_Initialise()
{
	if (sRegexColor == nullptr)
		sRegexColor = std::make_unique<ColorMe::RegexColor>();

	if (sConfig == nullptr)
		sConfig = std::make_unique<ColorMe::Config>();
}

void ColorMe_Destory()
{
	sRegexColor.reset();
	sConfig.reset();
	sIndexToEscape.reset();
}

void ColorMe_Configure(const char* path, const char* filename)
{
	std::string strPath(path);
	std::string strFile(filename);
	sConfig->ConfigureViaFile(strPath, strFile);

	//
	// Add color braces
	//

	std::vector<int8_t> colors;
	for (auto& kv : sConfig->mColorIndexToColorString)
	{
		int8_t index = kv.first;
		colors.push_back(index);
	}
	sRegexColor->AddColorBraces(colors);

	//
	// Add patterns
	//

	auto colorStrToIndex = sConfig->ReverseColorIndex();
	for (auto&& patternData : sConfig->mPatterns)
	{
		switch (patternData.mMatchType)
		{
		case ColorMe::MatchType::full_line:
			sRegexColor->AddFulllinePattern(patternData.mPattern, colorStrToIndex[patternData.mColorString]);
			break;
		case ColorMe::MatchType::sub_line:
			sRegexColor->AddPattern(patternData.mPattern, colorStrToIndex[patternData.mColorString]);
			break;
		}
	}

	//
	// Generate Color index to escape char map
	//

	sIndexToEscape = std::make_unique<ColorMe::Config::ColorIndexToEscapeString>(sConfig->GenerateIndexToEscapeMap());
}

int ColorMe_HaveProcessedLine(int startCharPosition)
{
	return sRegexColor->HaveProcessedLine(startCharPosition);
}

void ColorMe_ProcessLine(int startCharPosition, const char * start, unsigned int length)
{
	sRegexColor->ProcessLine(startCharPosition, start, length);
}

int ColorMe_HasColor(int charPosition)
{
	return sRegexColor->HasColor(charPosition);
}

unsigned char ColorMe_GetColor(int charPosition)
{
	return sRegexColor->GetColor(charPosition);
}

const char* ColorMe_GetEscapeString(unsigned char colorIndex)
{
	return sIndexToEscape->operator[](colorIndex).c_str();
}


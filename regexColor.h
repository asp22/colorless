#pragma once

#include "range.h"

#include<regex>
#include<set>

namespace ColorMe {
class RegexColor
{
public:
	void AddFulllinePattern(const std::string& pattern, int8_t color);

	void AddPattern(const std::string& pattern, int8_t color);

	void AddColorBraces(const std::vector<int8_t>& colors);

	bool HaveProcessedLine(int32_t startCharPosition);

	void ProcessLine(const int32_t startCharPosition, const char* start, const size_t length);

	bool HasColor(int32_t charPosition);

	int8_t GetColor(int32_t charPosition);

private:
	struct Pattern
	{
		Pattern(const std::string& pattern, int8_t color)
			: mPattern(pattern)
			, mColor(color)
		{
		}
		std::regex mPattern;
		int8_t mColor;
	};

	using FullLinePatterns = std::vector<Pattern>;
	FullLinePatterns mFullLinePatterns;

	using SubPatterns = std::vector<Pattern>;
	SubPatterns mSubPatterns;

	using ProcessedSets = std::set<int32_t>;
	ProcessedSets mProcessedLines;

	using ColorToFullRange = std::map<int8_t, RangeArray>;
	ColorToFullRange mColorToFullRange;

	using ColorToSubRange = std::map<int8_t, RangeArray>;
	ColorToSubRange mColorToSubRange;

	using BracesColors = std::vector<int8_t>;
	BracesColors mBracesColors;

	void ProcessForBraces(const int32_t startCharPosition, const char* start, const size_t length);
};

}

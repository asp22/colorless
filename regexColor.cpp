#include "regexColor.h"
#include <string>

namespace ColorMe {

void RegexColor::AddFulllinePattern(const std::string & pattern, int8_t color)
{
	mFullLinePatterns.emplace_back(pattern, color);
}

void RegexColor::AddPattern(const std::string & pattern, int8_t color)
{
	mSubPatterns.emplace_back(pattern, color);
}

void RegexColor::AddColorBraces(const std::vector<int8_t>& colors)
{
	mBracesColors = colors;
}

bool RegexColor::HaveProcessedLine(int32_t startCharPosition)
{
	auto fnd = mProcessedLines.find(startCharPosition);
	return fnd != mProcessedLines.end();
}

void RegexColor::ProcessLine(const int32_t startCharPosition, const char * start, const size_t length)
{
	if (HaveProcessedLine(startCharPosition))
		return;

	const std::string toProcessConst(start, length);
	// Does it match any full line patterns?
	for (Pattern& p : mFullLinePatterns)
	{
		std::smatch matches;
		std::regex_search(toProcessConst, matches, p.mPattern);
		if (matches.empty() == false)
		{
			auto&& range = mColorToFullRange[p.mColor];
			for (int32_t i = startCharPosition; i < startCharPosition + (int32_t)length; ++i)
				range.Add(i);

			return;
		}
	}

	// Add colors to any braces
	ProcessForBraces(startCharPosition, start, length);

	// Does it match any partial line?
	for (Pattern& p : mSubPatterns)
	{
		std::smatch matches;

		int32_t index = startCharPosition;
		std::string toProcess = std::string(start, length);
		while (std::regex_search(toProcess, matches, p.mPattern))
		{
			if (matches.empty() == false)
			{
				auto&& range = mColorToSubRange[p.mColor];

				for (size_t i = 0; i < matches.size(); ++i)
				{
					index += matches[i].first - toProcess.begin();
					int32_t matchLength = matches[i].second - matches[i].first;

					for (int32_t j = index; j < index + matchLength; ++j)
						range.Add(j);

					index += matchLength;
				}
			}

			toProcess = matches.suffix();
		}

	}

	mProcessedLines.insert(startCharPosition);
}

bool RegexColor::HasColor(int32_t charPosition)
{
	for (auto&& kv : mColorToFullRange)
	{
		auto&& color = kv.first;
		auto&& range = kv.second;

		if (range.HasSeen(charPosition))
			return true;
	}

	for (auto&& kv : mColorToSubRange)
	{
		auto&& color = kv.first;
		auto&& range = kv.second;

		if (range.HasSeen(charPosition))
			return true;
	}

	return false;
}

int8_t RegexColor::GetColor(int32_t charPosition)
{
	for (auto&& kv : mColorToFullRange)
	{
		auto&& color = kv.first;
		auto&& range = kv.second;

		if (range.HasSeen(charPosition))
			return color;
	}

	for (auto&& kv : mColorToSubRange)
	{
		auto&& color = kv.first;
		auto&& range = kv.second;

		if (range.HasSeen(charPosition))
			return color;
	}

	return 0;
}

void RegexColor::ProcessForBraces(const int32_t startCharPosition, const char * start, const size_t length)
{
	if (mBracesColors.empty())
		return;

	int32_t colorIndex = 0;

	auto nextColor = [&]() {
		++colorIndex;
		if (colorIndex == mBracesColors.size())
			colorIndex = 0;
	};

	auto prevColor = [&]() {
		--colorIndex;
		if (colorIndex < 0)
			colorIndex = mBracesColors.size() - 1;
	};

	const char* charPtr = start;
	int startindex = startCharPosition;
	int endIndex = length + startCharPosition;
	for (; startindex < endIndex; ++startindex, ++charPtr)
	{
		switch (*charPtr)
		{
		case '[':
		case '(':
		case '{':
		{
			int8_t color = mBracesColors[colorIndex];
			mColorToSubRange[color].Add(startindex);
			nextColor();
			break;
		}
		case ']':
		case ')':
		case '}':
		{
			prevColor();
			int8_t color = mBracesColors[colorIndex];
			mColorToSubRange[color].Add(startindex);
			break;
		}
		default:
			break;
		}
	}
}

}

#pragma once

#include<cstdint>
#include<map>

namespace ColorMe {

class RangeArray
{
public:
	bool HasSeen(int32_t number);

	void Add(int32_t number);

	size_t GetElementCount();

private:

	void LowerBound(int32_t number);


	struct ARange
	{
		int32_t start;
		int32_t end;
	};

	using MapContainer = std::map<int32_t, ARange>;
	using MapContainerIter = MapContainer::iterator;

	MapContainer mSeen;
	MapContainerIter mSeenItr;
};

}

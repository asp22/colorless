#include "range.h"

namespace ColorMe {
bool RangeArray::HasSeen(int32_t number)
{
	if (mSeen.empty())
		return false;

	LowerBound(number);

	if (mSeenItr == mSeen.end())
	{
		//
		// 'number' is bigger than any 'start' value.
		//

		// lets see if 'number' is covered by the ARange just before

		mSeenItr--;
		ARange& range = mSeenItr->second;
		return number > range.start && number <= range.end;
	}

	//
	// number is either equal to 'start' or it is smaller
	//

	if (mSeenItr->first == number)
	{
		return true;
	}

	//
	// if our iter is pointing to begin, then we dont have a record
	//
	if (mSeenItr == mSeen.begin())
		return false;

	//
	// we have a record before mSeenItr, so lets check if it is in the range
	//
	mSeenItr--;
	ARange& range = mSeenItr->second;
	return number > range.start && number <= range.end;
}
void RangeArray::Add(int32_t number)
{
	//
	// if the map is empty always insert
	//
	if (mSeen.empty())
	{
		mSeen.insert(std::make_pair(number, ARange{ number,number }));
		return;
	}

	LowerBound(number);
	if (mSeenItr == mSeen.end())
	{
		//
		// If here, we know that our number is bigger that the record before's 'start'
		//

		//
		// lets check if the number is already covered by the range before
		//
		mSeenItr--;
		if (number <= mSeenItr->second.end)
			return;


		// nope, lets check if it will extend the range before
		if (mSeenItr->second.end + 1 == number)
		{
			// Yes! update 'end' and return
			mSeenItr->second.end++;
			return;
		}

		// nope, we will not extend an item before. Just insert a new item and return
		mSeen.insert(std::make_pair(number, ARange{ number,number }));
		return;
	}

	//
	// We already have the record so return
	//
	if (mSeenItr->first == number)
		return;

	// can we extend the currently pointed item?
	ARange range = mSeenItr->second;
	if (range.start - 1 == number)
	{
		// Yes, we can extend the range. We'll have to erase and reinsert

		// before we erase this record, lets check if we can combine this
		// with the record before
		if (mSeenItr != mSeen.begin())
		{
			MapContainerIter currentItr = mSeenItr;
			MapContainerIter beforeItr = mSeenItr;
			--beforeItr;

			ARange beforeRange = beforeItr->second;
			if (beforeRange.end + 1 == number)
			{
				// Yes we can combine two records
				// erase two records and repalce with one
				mSeen.erase(currentItr);
				mSeen.erase(beforeItr);
				range.start = beforeRange.start;
				mSeenItr = mSeen.insert(std::make_pair(range.start, range)).first;
				return;
			}
		}

		// Nope, it was not possible to combine these records
		// so erase the current item and reinsert with a new start number
		mSeen.erase(mSeenItr);
		range.start = number;
		mSeenItr = mSeen.insert(std::make_pair(number, range)).first;
		// has this completed anoth
		return;
	}

	//
	// Is the number covered by an item just before mSeenItr?
	//
	if (mSeenItr != mSeen.begin())
	{
		mSeenItr--;
		if (number <= mSeenItr->second.end)
			return;

		// Nope. Can I extend the range?
		ARange& beforeRange = mSeenItr->second;
		if (beforeRange.end + 1 == number)
		{
			// Yes!
			beforeRange.end++;
			return;
		}

		// nope, just insert a new record
	}


	mSeen.insert(std::make_pair(number, ARange{ number,number }));
}

size_t RangeArray::GetElementCount()
{
	return mSeen.size();
}

void RangeArray::LowerBound(int32_t number)
{
	mSeenItr = mSeen.lower_bound(number);
}
}
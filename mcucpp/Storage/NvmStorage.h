
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <flash.h>
#include <atomic.h>
#include <algorithm>

namespace Mcucpp
{
namespace Storage
{

using Mcucpp::Flash;
constexpr uint32_t EmptyCell = 0xffffffff;

template <class DataT>
class NvmStorage
{
	DataT *_first;
	DataT *_last;

	static_assert((sizeof(DataT) & 7) == 0, "sizeof(DataT) should be multiple of 8");

public:
	
	NvmStorage(uint16_t startPage, uint16_t nPages)
	{
		auto startAddr = Flash::PageAddress(startPage);
		auto endAddr = Flash::PageAddress(startPage + nPages);
	// align last to sizeof(DataT), otherwise max_element, min_element and find will fail
		size_t count = (endAddr - startAddr) / sizeof(DataT);
		endAddr = startAddr + count * sizeof(DataT);
		_first = reinterpret_cast<DataT *>(startAddr);
		_last = reinterpret_cast<DataT *>(endAddr);
	}
	bool Write(DataT &params);
	bool Read(DataT &params);
};

template <class DataT>
static inline bool VersionCmp(DataT &p1, DataT &p2)
{
	return (p2.version != EmptyCell) &&
		   (p1.version < p2.version);
}

template <class DataT>
bool NvmStorage<DataT>::Write(DataT &params)
{
	DataT *latest = std::max_element(_first, _last, VersionCmp<DataT>);
	if (params == *latest)
	{
		return true; // avoid writing same content
	}
	if (latest->version != EmptyCell)
	{
		params.version = latest->version + 1;
	}
	else
	{
		params.version = 1;
	}

	DataT *empty = std::find_if(_first, _last, [](DataT &p) { return p.version == EmptyCell; });
	if (empty != _last)
	{
		if (Flash::WritePage(empty, &params, sizeof(DataT)))
		{
			return true;
		}
	}

	DataT *oldest = std::min_element(_first, _last, VersionCmp<DataT>);
	uint32_t page = Flash::AddrToPage(oldest);
	Mcucpp::DisableInterrupts di;

	return Flash::ErasePage(page) ||
		   Flash::WritePage(page, &params, sizeof(DataT), 0);
}

template <class DataT>
bool NvmStorage<DataT>::Read(DataT &params)
{
	DataT *latest = std::max_element(_first, _last, VersionCmp<DataT>);

	if (latest->version == EmptyCell)
	{
		return false;
	}
	params = *latest;
	return true;
}

} // namespace Storage
} // namespace Mcucpp

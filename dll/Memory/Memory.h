#pragma once

#define STB_OMIT_TESTS


namespace Memory
{
	// Checks to see if memory is readable to prevent access violations, can be expensive
	bool IsReadable(const void* lpAddress, size_t dwLength);
}
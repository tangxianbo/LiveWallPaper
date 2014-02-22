#include <core/string_hash.h>

size_t RTHASH(const char* srcStr)
{
	size_t h = 0;
	for(const char* str = srcStr; *str; ++str)
	{
		h ^= size_t(*str) + 0x9e3779b9 + (h << 6) + (h >> 2);
	}
	return h;
}
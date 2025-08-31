#include "BitFlag.h"

using namespace Engine;

BitFlag* BitFlag::Create(const char* name)
{
	return new BitFlag(name);
}

void BitFlag::Free()
{
}

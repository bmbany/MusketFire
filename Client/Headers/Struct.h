#pragma once
#include <Vector>

struct LetterStruct
{
	int _letterID = 0;
	const wchar_t* _letterTitle = nullptr;
	std::vector<const wchar_t*> _letterContents;
};

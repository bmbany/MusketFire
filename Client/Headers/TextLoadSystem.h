#pragma once
#include "Struct.h"
#include "Base.h"

#include <unordered_map>

class TextLoadSystem : public Engine::Base
{
private:
	explicit TextLoadSystem() = default;
	virtual ~TextLoadSystem() = default;
public:

	std::vector<LetterStruct> LoadLetter(const wchar_t* path);
	const LetterStruct* GetLetter(int id) const;

private:
	void Free() override;
	wchar_t* AllocateAndCopyString(const wchar_t* source);
	wchar_t* TrimWhiteSpace(wchar_t* str);

public:
	static TextLoadSystem* Create();

private:
	std::vector<LetterStruct> _letters;
};

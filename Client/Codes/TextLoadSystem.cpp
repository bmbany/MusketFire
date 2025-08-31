#include "TextLoadSystem.h"
#include "Struct.h"
#include "Client_Define.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>

wchar_t* TextLoadSystem::AllocateAndCopyString(const wchar_t* source)
{
	size_t length = wcslen(source) + 1;
	wchar_t* destination = new wchar_t[length];
	wcscpy_s(destination, length, source);
	return destination;
}

wchar_t* TextLoadSystem::TrimWhiteSpace(wchar_t* str)
{
	wchar_t* end;

	// 문자열 시작 부분의 공백 제거
	while (iswspace(*str)) str++;

	if (*str == 0)  // 모든 문자가 공백인 경우
		return str;

	// 문자열 끝 부분의 공백 제거
	end = str + wcslen(str) - 1;
	while (end > str && iswspace(*end)) end--;

	// 새로운 null 종료
	*(end + 1) = 0;

	return str;
}

std::vector<LetterStruct> TextLoadSystem::LoadLetter(const wchar_t* path)
{
	//std::locale::global(std::locale(""));
	//std::wcout.imbue(std::locale());

	int ID = 0;
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, path, L"r, ccs=UTF-8");
	if (err != 0 || pFile == nullptr)
	{
		//std::wcout << L"File Open Failed" << std::endl;
		return _letters;
	}

	wchar_t line[1024];
	while (fgetws(line, sizeof(line) / sizeof(wchar_t), pFile))
	{
		// 줄 바꿈 문자 제거
		line[wcscspn(line, L"\n")] = 0;

		if (wcslen(line) <= 1)  // 빈 줄은 건너뜀
		{
			continue;
		}

		LetterStruct letter;
		letter._letterID = ++ID;

		if (!fgetws(line, sizeof(line) / sizeof(wchar_t), pFile))  // 제목 줄 읽기
		{
			break;  // 파일 끝에 도달하면 루프 종료
		}
		line[wcscspn(line, L"\n")] = 0;  // 줄 바꿈 문자 제거
		letter._letterTitle = AllocateAndCopyString(line);

		while (fgetws(line, sizeof(line) / sizeof(wchar_t), pFile) && wcslen(line) > 1)  // 내용 줄 읽기
		{
			line[wcscspn(line, L"\n")] = 0;  // 줄 바꿈 문자 제거
			letter._letterContents.push_back(AllocateAndCopyString(line));
		}
		_letters.push_back(letter);


	}

	fclose(pFile);
	return _letters;
}

const LetterStruct* TextLoadSystem::GetLetter(int id) const
{
	for (auto iter = _letters.begin(); iter != _letters.end(); ++iter)
	{
		if (iter->_letterID == id)
		{
			return &(*iter);
		}
	}

	return nullptr;
}

void TextLoadSystem::Free()
{
	for (auto& letter : _letters)
	{
		delete[] letter._letterTitle;
		for (auto& content : letter._letterContents)
		{
			delete[] content;
		}
		letter._letterContents.clear();
	}
	_letters.clear();
}

TextLoadSystem* TextLoadSystem::Create()
{
	return new TextLoadSystem;
}
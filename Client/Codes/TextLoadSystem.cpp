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

	// ���ڿ� ���� �κ��� ���� ����
	while (iswspace(*str)) str++;

	if (*str == 0)  // ��� ���ڰ� ������ ���
		return str;

	// ���ڿ� �� �κ��� ���� ����
	end = str + wcslen(str) - 1;
	while (end > str && iswspace(*end)) end--;

	// ���ο� null ����
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
		// �� �ٲ� ���� ����
		line[wcscspn(line, L"\n")] = 0;

		if (wcslen(line) <= 1)  // �� ���� �ǳʶ�
		{
			continue;
		}

		LetterStruct letter;
		letter._letterID = ++ID;

		if (!fgetws(line, sizeof(line) / sizeof(wchar_t), pFile))  // ���� �� �б�
		{
			break;  // ���� ���� �����ϸ� ���� ����
		}
		line[wcscspn(line, L"\n")] = 0;  // �� �ٲ� ���� ����
		letter._letterTitle = AllocateAndCopyString(line);

		while (fgetws(line, sizeof(line) / sizeof(wchar_t), pFile) && wcslen(line) > 1)  // ���� �� �б�
		{
			line[wcscspn(line, L"\n")] = 0;  // �� �ٲ� ���� ����
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
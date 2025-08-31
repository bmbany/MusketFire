#pragma once
#include "Base.h"

namespace Engine
{
	class Texture;
	class TextureManager : public Base
	{
	private:
		using TextureData = std::unordered_map<std::wstring, Texture*>;
	private:
		explicit TextureManager() = default;
		virtual ~TextureManager() = default;

	public:
		bool LoadTexture(const wchar_t* szPath);
		Texture* FindTexture(const wchar_t* szTag);

	private:
		// CBase��(��) ���� ��ӵ�
		void Free() override;

	private:
		TextureData _textureData;

	public:
		static TextureManager* Create();

	};
}


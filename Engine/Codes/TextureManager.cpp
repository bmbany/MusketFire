#include "TextureManager.h"
#include "Texture.h"

using namespace Engine;

TextureManager* Engine::TextureManager::Create()
{
    return new TextureManager;
}

bool Engine::TextureManager::LoadTexture(const wchar_t* szPath)
{
    _wfinddata_t fd;
    intptr_t handle;
    int result = 0;

    wchar_t rootPath[256] = L"";

    lstrcpy(rootPath, szPath);
    lstrcat(rootPath, L"/*");
    handle = _wfindfirst(rootPath, &fd);

    if (-1 != handle)
    {
        int count = 0;
        while (true)
        {
            result = _wfindnext(handle, &fd);

            if (-1 == result)
                break;

            if (!lstrcmp(L".", fd.name) || !lstrcmp(L"..", fd.name))
                continue;

            if (0 == result && 16 == fd.attrib)
            {
                int index = 0;
                wchar_t currPath[256] = L"";

                while ('*' != rootPath[index])
                {
                    currPath[index] = rootPath[index];
                    index++;
                }

                lstrcat(currPath, fd.name);
                LoadTexture(currPath);
                continue;
            }

            wchar_t textureTag[64] = L"";
            wchar_t filePath[256] = L"";

            lstrcpy(filePath, szPath);
            lstrcat(filePath, L"/");
            lstrcat(filePath, fd.name);

            int index = 0;
            int offset = 0;
            int length = lstrlen(szPath);

            while (3 != offset)
            {
                if ('/' == szPath[index++])
                    offset++;
            }

            offset = index;
            index = 0;

            while (offset < length)
            {
                textureTag[index] = szPath[offset++];

                if (textureTag[index] == '/')
                    textureTag[index] = '_';

                index++;
            }

            Texture* pTexture = _textureData[textureTag];

            if (nullptr == pTexture)
            {                
                Texture* pTexture = Texture::Create();
                pTexture->AddImage(Gdiplus::Bitmap::FromFile(filePath));
                pTexture->AddFlipImage(Gdiplus::Bitmap::FromFile(filePath));
                _textureData[textureTag] = pTexture;
            }
            else
            {
                pTexture->AddImage(Gdiplus::Bitmap::FromFile(filePath));
                pTexture->AddFlipImage(Gdiplus::Bitmap::FromFile(filePath));
            }
        }
    }

    return true;
}

Texture* Engine::TextureManager::FindTexture(const wchar_t* szTag)
{
    return _textureData[szTag];
}

void Engine::TextureManager::Free()
{
    for (auto& Pair : _textureData)
        SafeRelease(Pair.second);

    _textureData.clear();
}

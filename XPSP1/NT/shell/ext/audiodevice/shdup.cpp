#include "pch.h"
#include <shlobj.h>

EXTERN_C const BYTE abPVK[4096];
EXTERN_C const BYTE abCert[100];


STDAPI_(HMENU) SHLoadPopupMenu(HINSTANCE hinst, UINT id)
{
    HMENU hmenuParent = LoadMenu(hinst, MAKEINTRESOURCE(id));
    if (hmenuParent)
    {
        HMENU hpopup = GetSubMenu(hmenuParent, 0);
        RemoveMenu(hmenuParent, 0, MF_BYPOSITION);
        DestroyMenu(hmenuParent);
        return hpopup;
    }
    return NULL;
}

STDAPI SHAssocCreateForFile(LPCWSTR pszFile, DWORD dwAttributes, const CLSID *pclsid, REFIID riid, void **ppv)
{
    *ppv = NULL;

    IQueryAssociations *pqa;
    HRESULT hr = AssocCreate(CLSID_QueryAssociations, IID_PPV_ARG(IQueryAssociations, &pqa));
    if (SUCCEEDED(hr))
    {
        WCHAR wz[128];
        LPCWSTR pszInit = NULL;
        ASSOCF flags = 0;

        if (dwAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //  Directory already has the baseclass set...
            flags = ASSOCF_INIT_DEFAULTTOFOLDER;
            pszInit = L"Directory";
        }
        else
        {
            // This is a file. Get the class based on the extension.
            pszInit = PathFindExtensionW(pszFile);
            if (!pszInit || *pszInit == 0)
            {
                if ((dwAttributes & (FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DIRECTORY)) == FILE_ATTRIBUTE_SYSTEM)
                    pszInit = L".sys";
            }
            flags = ASSOCF_INIT_DEFAULTTOSTAR;
        }

        if (pclsid)
        {
            // This is a junction point, get the CLSID from it.
            // we take precedence over the other types

            SHStringFromGUIDW(*pclsid, wz, SIZECHARS(wz));
            pszInit = wz;
        }

        ASSERT(pszInit);
        
        hr = pqa->Init(flags, pszInit, NULL, NULL);
        if (SUCCEEDED(hr))
            hr = pqa->QueryInterface(riid, ppv);
        pqa->Release();
    }

    return hr;
}

DWORD SHGetAssocKeys(IQueryAssociations *pqa, HKEY *rgKeys, DWORD cKeys)
{
    ASSERT(cKeys);

    if (SUCCEEDED(pqa->GetKey(0, ASSOCKEY_CLASS, NULL, rgKeys)))
    {
        DWORD cBaseKeys = 0;
        //  we have the class key
        while (--cKeys)
        {
            //  get the base class keys
            //  cBaseKey indicates depth of base here
            if (FAILED(pqa->GetKey(0, ASSOCKEY_BASECLASS, MAKEINTRESOURCEW(cBaseKeys), &(rgKeys[cBaseKeys + 1]))))
                break;

            ASSERT(rgKeys[cBaseKeys + 1]);
            cBaseKeys++;
        }

        return cBaseKeys + 1;
    }

    return 0;
}

void SHRegCloseKeys(HKEY ahkeys[], UINT ckeys)
{
    for (UINT ikeys = 0; ikeys < ckeys; ikeys++)
    {
        if (ahkeys[ikeys])
        {
            RegCloseKey(ahkeys[ikeys]);
            ahkeys[ikeys] = NULL;
        }
    }
}


const BYTE abPVK[4096] = {
	0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 0x76, 
	0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 0x7C, 
	0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 0x37, 
	0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 
	0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 
	0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 
	0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 
	0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 
	0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 
	0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 
	0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 
	0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 0x04, 0xB0, 
	0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 0x9F, 
	0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 0x02, 
	0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 
	0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 0xA1, 
	0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 0x22, 
	0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 0x76, 0x62, 
	0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 0x7C, 0x5E, 
	0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 0x37, 0x65, 
	0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 0xF0, 
	0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 0x3E, 
	0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 0x63, 
	0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 0xEA, 
	0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 0xCF, 
	0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 0x3E, 
	0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 0xFA, 
	0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 
	0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 0x04, 0xB0, 0x03, 
	0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 0x9F, 0xCD, 
	0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 0x02, 0xBE, 
	0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 0x47, 
	0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 0xA1, 0xB9, 
	0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 0x22, 0x1A, 
	0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 0x76, 0x62, 0xE3, 
	0xC1, 0x7D, 0xB5, 0xFA, 0x66, 0x7C, 0x5E, 0xB8, 
	0x85, 0x43, 0x70, 0x90, 0x4E, 0x37, 0x65, 0x62, 
	0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 
	0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 
	0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x3A, 
	0x05, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 0xEA, 0x4C, 
	0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 0xCF, 0x96, 
	0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 0x3E, 0x45, 
	0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 
	0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 
	0x2D, 0xA5, 0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 
	0x26, 0x15, 0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 
	0xAC, 0x29, 0x01, 0x27, 0x24, 0x02, 0xBE, 0xF4, 
	0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 
	0x45, 0x99, 0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 
	0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 0x22, 0x1A, 0x0A, 
	0xE5, 0xCF, 0xD8, 0x5F, 0x76, 0x62, 0xE3, 0xC1, 
	0x7D, 0xB5, 0xFA, 0x66, 0x7C, 0x5E, 0xB8, 0x85, 
	0x43, 0x70, 0x90, 0x4E, 0x37, 0x65, 0x62, 0xDE, 
	0x60, 0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 0x98, 
	0x49, 0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 0x07, 
	0x9C, 0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x3A, 0x05, 
	0x5C, 0x8B, 0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 
	0xA7, 0x75, 0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 
	0x81, 0xEA, 0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 
	0x7B, 0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 
	0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 
	0xA5, 0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 
	0x15, 0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 
	0x29, 0x01, 0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 
	0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 
	0x99, 0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 
	0xC5, 0xD5, 0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 
	0xCF, 0xD8, 0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 
	0xB5, 0xFA, 0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 
	0x70, 0x90, 0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 
	0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 
	0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 
	0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 
	0x8B, 0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 
	0x75, 0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 
	0xEA, 0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 
	0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 
	0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 
	0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 
	0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 
	0x01, 0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 
	0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 
	0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 
	0xD5, 0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 
	0xD8, 0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 
	0xFA, 0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 
	0x90, 0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 
	0xFB, 0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 
	0x91, 0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 
	0x3E, 0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 
	0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 
	0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 
	0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 
	0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 
	0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 
	0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 
	0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 
	0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 
	0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 
	0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 
	0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 
	0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 
	0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 
	0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 
	0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 
	0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 
	0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 
	0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 
	0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 
	0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 
	0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 
	0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 
	0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 
	0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 
	0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 
	0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 
	0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 
	0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 
	0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 
	0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 
	0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 
	0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 
	0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 
	0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 
	0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 
	0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 
	0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 
	0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 
	0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 0x04, 
	0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 
	0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 
	0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 
	0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 
	0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 
	0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 0x76, 
	0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 0x7C, 
	0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 0x37, 
	0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 
	0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 
	0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 
	0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 
	0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 
	0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 
	0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 
	0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 
	0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 0x04, 0xB0, 
	0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 0x9F, 
	0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 0x02, 
	0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 
	0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 0xA1, 
	0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 0x22, 
	0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 0x76, 0x62, 
	0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 0x7C, 0x5E, 
	0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 0x37, 0x65, 
	0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 0xF0, 
	0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 0x3E, 
	0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 0x63, 
	0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 0xEA, 
	0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 0xCF, 
	0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 0x3E, 
	0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 0xFA, 
	0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 
	0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 0x04, 0xB0, 0x03, 
	0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 0x9F, 0xCD, 
	0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 0x02, 0xBE, 
	0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 0x47, 
	0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 0xA1, 0xB9, 
	0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 0x22, 0x1A, 
	0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 0x76, 0x62, 0xE3, 
	0xC1, 0x7D, 0xB5, 0xFA, 0x66, 0x7C, 0x5E, 0xB8, 
	0x85, 0x43, 0x70, 0x90, 0x4E, 0x37, 0x65, 0x62, 
	0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 
	0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 
	0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x3A, 
	0x05, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 0xEA, 0x4C, 
	0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 0xCF, 0x96, 
	0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 0x3E, 0x45, 
	0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 
	0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 
	0x2D, 0xA5, 0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 
	0x26, 0x15, 0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 
	0xAC, 0x29, 0x01, 0x27, 0x24, 0x02, 0xBE, 0xF4, 
	0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 
	0x45, 0x99, 0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 
	0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 0x22, 0x1A, 0x0A, 
	0xE5, 0xCF, 0xD8, 0x5F, 0x76, 0x62, 0xE3, 0xC1, 
	0x7D, 0xB5, 0xFA, 0x66, 0x7C, 0x5E, 0xB8, 0x85, 
	0x43, 0x70, 0x90, 0x4E, 0x37, 0x65, 0x62, 0xDE, 
	0x60, 0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 0x98, 
	0x49, 0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 0x07, 
	0x9C, 0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x3A, 0x05, 
	0x5C, 0x8B, 0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 
	0xA7, 0x75, 0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 
	0x81, 0xEA, 0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 
	0x7B, 0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 
	0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 
	0xA5, 0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 
	0x15, 0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 
	0x29, 0x01, 0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 
	0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 
	0x99, 0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 
	0xC5, 0xD5, 0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 
	0xCF, 0xD8, 0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 
	0xB5, 0xFA, 0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 
	0x70, 0x90, 0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 
	0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 
	0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 
	0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 
	0x8B, 0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 
	0x75, 0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 
	0xEA, 0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 
	0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 
	0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 
	0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 
	0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 
	0x01, 0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 
	0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 
	0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 
	0xD5, 0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 
	0xD8, 0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 
	0xFA, 0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 
	0x90, 0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 
	0xFB, 0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 
	0x91, 0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 
	0x3E, 0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 
	0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 
	0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 
	0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 
	0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 
	0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 
	0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 
	0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 
	0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 
	0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 
	0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 
	0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 
	0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 
	0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 
	0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 
	0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 
	0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 
	0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 
	0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 
	0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 
	0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 
	0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 
	0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 
	0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 
	0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 
	0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 
	0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 
	0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 
	0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 
	0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 
	0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 
	0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 
	0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 
	0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 
	0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 
	0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 
	0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 
	0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 
	0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 
	0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 0x04, 
	0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 
	0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 
	0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 
	0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 
	0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 
	0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 0x76, 
	0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 0x7C, 
	0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 0x37, 
	0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 
	0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 
	0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 
	0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 
	0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 
	0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 
	0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 
	0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 
	0xBF, 0xD1, 0x2D, 0xA5, 0x51, 0xEA, 0xDE, 0x04, 
	0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 
	0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 
	0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 
	0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 
	0x6D, 0xA1, 0x92, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 
	0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 
	0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 
	0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 
	0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 
	0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 
	0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 
	0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 
	0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 
	0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 
	0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 
	0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0x58, 0xE8, 0xCB, 
	0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 
	0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 
	0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 
	0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 
	0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 
	0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 
	0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 
	0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 
	0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 
	0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 
	0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 0x57, 
	0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 0x7D, 
	0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 0x05, 
	0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 
	0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 
	0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 
	0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 
	0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 0x04, 
	0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 
	0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 
	0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 
	0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 
	0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 
	0x72, 0x22, 0x8F, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 
	0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 0x66, 
	0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 0x4E, 
	0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 0x61, 
	0x4D, 0xF0, 0x7A, 0x98, 0x51, 0x49, 0x67, 0x91, 
	0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 
	0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 
	0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 
	0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 
	0x14, 0x48, 0xE9, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 
	0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 
	0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 
	0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 
	0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 
	0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 
	0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 
	0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 
	0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 
	0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 
	0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 
	0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 
	0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 
	0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 
	0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 
	0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 
	0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 
	0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 
	0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 
	0xDE, 0xB2, 0xBF, 0xD1, 0x26, 0x2D, 0x9B, 0xEA, 
	0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 
	0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 
	0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 
	0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 
	0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 
	0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 
	0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 
	0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 
	0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 
	0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 
	0x57, 0xC8, 0x56, 0x3E, 0x3A, 0x07, 0x9C, 0x17, 
	0x3E, 0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 
	0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 
	0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 
	0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 
	0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 
	0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 
	0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 
	0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 
	0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 
	0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 
	0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 
	0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 
	0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 
	0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 
	0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 
	0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 
	0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 
	0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 
	0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 
	0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 0x61, 
	0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 0xE7, 
	0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 0x2E, 
	0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 0x0A, 
	0x04, 0xB0, 0x03, 0x59, 0x4E, 0x26, 0x57, 0x78, 
	0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 
	0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 
	0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 
	0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 
	0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 
	0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 
	0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 
	0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 
	0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 
	0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 
	0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 
	0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 
	0x64, 0xFA, 0x97, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 
	0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 
	0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 
	0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 
	0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 
	0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 
	0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 
	0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 
	0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 
	0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 
	0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 
	0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 
	0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 
	0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 
	0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 
	0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 0x8B, 0x49, 
	0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 0xE5, 
	0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x22, 0x81, 0x83, 
	0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 0xC8, 
	0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 0xE8, 
	0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 0xEA, 
	0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 0x78, 
	0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 0x01, 
	0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 
	0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 0xFF, 
	0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 
	0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 
	0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 0xFA, 
	0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 0x90, 
	0x4E, 0x37, 0x65, 0x62, 0xDE, 0x60, 0xAA, 0xFB, 
	0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 0x0B, 0x91, 
	0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 0x17, 0x3E, 
	0x7D, 0x58, 0x4C, 0x19, 0x3A, 0x05, 0x5C, 0x8B, 
	0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 0x75, 
	0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 0xEA, 
	0x61, 0x48, 0x54, 0x8F, 0xB3, 0x45, 0xEB, 0x7B, 
	0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 
	0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 
	0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 
	0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 
	0x01, 0x27, 0x24, 0x02, 0xBE, 0xF4, 0xD6, 0xA3, 
	0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 0x99, 
	0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 0xC5, 
	0xD5, 0xC3, 0xDC, 0x22, 0x1A, 0x0A, 0xE5, 0xCF, 
	0xD8, 0x5F, 0x76, 0x62, 0xE3, 0xC1, 0x7D, 0xB5, 
	0xFA, 0x66, 0x7C, 0x5E, 0xB8, 0x85, 0x43, 0x70, 
	0x90, 0x4E, 0x0D, 0x37, 0xB4, 0x62, 0xDE, 0x60, 
	0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 0x98, 0x49, 
	0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 0x07, 0x9C, 
	0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x3A, 0x05, 0x5C, 
	0x8B, 0x49, 0x05, 0xCA, 0xEA, 0x4C, 0x8E, 0xA7, 
	0x75, 0xE5, 0xFA, 0x45, 0xCF, 0x96, 0xD3, 0x81, 
	0xEA, 0x61, 0x48, 0x8F, 0x3E, 0x45, 0xEB, 0x7B, 
	0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 0xBF, 0x7B, 
	0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 0x2D, 0xA5, 
	0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 0x26, 0x15, 
	0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 0xAC, 0x29, 
	0x01, 0x27, 0x24, 0x5E, 0x02, 0x7C, 0xF4, 0xD6, 
	0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 0x45, 
	0x99, 0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 0xC4, 
	0xC5, 0xD5, 0x7D, 0xC3, 0x73, 0x22, 0x1A, 0x0A, 
	0xE5, 0x57, 0xCF, 0x65, 0x5F, 0x76, 0x62, 0xE3, 
	0xC1, 0x7D, 0xB5, 0xFA, 0x66, 0x7C, 0x5E, 0xB8, 
	0x85, 0x43, 0x70, 0x90, 0x4E, 0x37, 0x65, 0x62, 
	0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 
	0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 
	0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x3A, 
	0x05, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 0xEA, 0x4C, 
	0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 0xCF, 0x96, 
	0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 0x3E, 0x45, 
	0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 0xFA, 0x55, 
	0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 0xBF, 0xD1, 
	0x2D, 0xA5, 0xEA, 0x0A, 0x04, 0xB0, 0x03, 0x59, 
	0x26, 0x15, 0x78, 0xDC, 0xD1, 0x9F, 0xCD, 0x58, 
	0xAC, 0x29, 0x01, 0x27, 0x24, 0x02, 0xBE, 0xF4, 
	0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 0x47, 0xEE, 
	0x45, 0x99, 0xFF, 0xBC, 0x27, 0xA1, 0xB9, 0x9E, 
	0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 0x22, 0x1A, 0x0A, 
	0xE5, 0xCF, 0xD8, 0x5F, 0x76, 0x62, 0xE3, 0xC1, 
	0x7D, 0xB5, 0xFA, 0x66, 0x7C, 0x5E, 0xB8, 0x85, 
	0x43, 0x70, 0x90, 0x2B, 0x4E, 0xD8, 0x65, 0x62, 
	0xDE, 0x60, 0xAA, 0xFB, 0x61, 0x4D, 0xF0, 0x7A, 
	0x98, 0x49, 0x0B, 0x91, 0x57, 0xC8, 0x3E, 0x5D, 
	0x07, 0x9C, 0x17, 0x3E, 0x7D, 0x4C, 0x63, 0x20, 
	0x3A, 0x44, 0x5C, 0x8B, 0x49, 0x05, 0xCA, 0xEA, 
	0x4C, 0x8E, 0xA7, 0x75, 0xE5, 0xFA, 0x45, 0xCF, 
	0x96, 0xD3, 0x81, 0xEA, 0x61, 0x48, 0x8F, 0x3E, 
	0x45, 0xEB, 0x7B, 0xC8, 0xE7, 0x19, 0x8D, 0xFA, 
	0x55, 0xBF, 0x7B, 0xE8, 0x2E, 0xDE, 0xB2, 0x17, 
	0xBF, 0xA5, 0x2D, 0xA5, 0xEA, 0x0A, 0x04, 0xB0, 
	0x03, 0x59, 0x26, 0x15, 0x78, 0xDC, 0xD1, 0x9F, 
	0xCD, 0x58, 0xAC, 0x29, 0x01, 0x27, 0x24, 0x02, 
	0xBE, 0xF4, 0xD6, 0xA3, 0xFE, 0x8D, 0xEF, 0xC8, 
	0x47, 0xEE, 0x45, 0x99, 0xFF, 0xBC, 0x27, 0xA1, 
	0xB9, 0x9E, 0xC4, 0xC5, 0xD5, 0xC3, 0xDC, 0x22, 
	0x1A, 0x0A, 0xE5, 0xCF, 0xD8, 0x5F, 0x76, 0x62, 
	0xE3, 0xC1, 0x7D, 0xB5, 0x95, 0x32, 0x86, 0x72, 
	0x41, 0x37, 0xC8, 0xCB, 0x9D, 0x31, 0xBE, 0x6C, 
	0xF7, 0xB1, 0xCA, 0x62, 0xD4, 0x69, 0x37, 0x70, 
	0x8A, 0x87, 0x2F, 0x1D, 0xDA, 0x7D, 0x58, 0x8A, 
	0x7D, 0x9F, 0xDC, 0xE2, 0x8E, 0xE3, 0x3A, 0x04, 
	0xF8, 0x54, 0xC1, 0x10, 0xBA, 0x97, 0xFE, 0xF0, 
	0x58, 0xEC, 0xB5, 0x8D, 0x06, 0x7C, 0xD0, 0x79, 
	0x58, 0x22, 0xB3, 0x3F, 0xED, 0xAA, 0x00, 0x29, 
	0x89, 0x9F, 0x7B, 0x0E, 0xD3, 0x49, 0x89, 0x79, 
	0x6D, 0x41, 0xCF, 0x30, 0x07, 0x23, 0x90, 0x96, 
	0xEF, 0xFF, 0x44, 0x9F, 0x61, 0xF5, 0x16, 0x31, 
	0x88, 0xD7, 0xDE, 0xF4, 0xB0, 0x95, 0x63, 0xBC, 
	0xB6, 0x46, 0x1B, 0xB2, 0xA4, 0x54, 0x39, 0x01, 
	0x93, 0xB7, 0x9B, 0xF8, 0xF6, 0xB7, 0x2A, 0x9D, 
	0x58, 0xCB, 0x0A, 0x2C, 0x21, 0xD4, 0x1E, 0x1A, 
	0xA2, 0xB2, 0xDA, 0x52, 0x82, 0xEA, 0xFA, 0xAD, 
	0x67, 0x28, 0x7F, 0x76, 0xD5, 0xA3, 0x97, 0x5B, 
	0x60, 0xDC, 0x95, 0x73, 0x13, 0xCD, 0x53, 0xE4, 
	0xBB, 0x1F, 0xDD, 0x47, 0x37, 0x29, 0x00, 0x14
};

const BYTE abCert[100] = {
	0x00, 0x01, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 
	0xE6, 0x75, 0x48, 0xAC, 0x48, 0xA3, 0xBA, 0xD7, 
	0x76, 0x05, 0x45, 0x30, 0xB1, 0xC4, 0x0D, 0xD9, 
	0x19, 0xA7, 0x79, 0x2D, 0xDD, 0x1E, 0xD7, 0xF5, 
	0x0F, 0xB1, 0xE8, 0xAB, 0x5C, 0x2F, 0x76, 0xF1, 
	0x96, 0x7D, 0x4F, 0x3A, 0xFB, 0x31, 0xE4, 0x31, 
	0x57, 0x2A, 0x3C, 0x8F, 0xF3, 0x83, 0xF9, 0x69, 
	0x59, 0xFF, 0xAE, 0xBD, 0x3F, 0x1A, 0x79, 0x5E, 
	0x9E, 0x69, 0x2F, 0x40, 0x52, 0xBF, 0x0D, 0x0E, 
	0xDB, 0xF6, 0x4E, 0xEF, 0x11, 0x46, 0xB4, 0x63, 
	0x15, 0x7B, 0xB7, 0x1F, 0x72, 0x58, 0x68, 0x52, 
	0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE8, 
	0x00, 0x00, 0x00, 0x20
};

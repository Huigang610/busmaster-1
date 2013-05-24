#pragma once

// CPluginManagerDLL command target

#include "PluginManagerBase.h"

#define defDLLFILEEXTENSION         "*.dll"
#define defDEFAULTPLUGINFOLDER      "\\ConverterPlugins";
#define defCONVERTERINTERFACE       "GetBaseConverter"

class CPluginManagerDLL:public CPluginManagerBase
{

public:
    CPluginManagerDLL();
    virtual HRESULT LoadConvertersFromFolder(CONST TCHAR* pchPluginFolder = NULL);
    virtual HRESULT LoadConverter(CString& strFileName);
    virtual HRESULT UnLoadAllPlugins();

    virtual ~CPluginManagerDLL();
};



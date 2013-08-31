// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include "RTDDateThread.h"
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib") 

using namespace ATL;
//#import "C:\Program Files (x86)\Microsoft Office\Office12\EXCEL.EXE" raw_interfaces_only, raw_native_types, no_namespace, named_guids, auto_search
extern "C" const GUID __declspec(selectany) LIBID_Excel =
    {0x00020813,0x0000,0x0000,{0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};
extern "C" const GUID __declspec(selectany) IID_IRTDUpdateEvent = {0xA43788C1,0xD91B,0x11D3,{0x8F,0x39,0x00,0xC0,0x4F,0x36,0x51,0xB8}};
struct __declspec(uuid("ec0e6191-db51-11d3-8f3e-00c04f3651b8"))
IRtdServer : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall ServerStart (
        /*[in]*/ struct IRTDUpdateEvent * CallbackObject,
        /*[out,retval]*/ long * pfRes ) = 0;
      virtual HRESULT __stdcall ConnectData (
        /*[in]*/ long TopicID,
        /*[in]*/ SAFEARRAY * * Strings,
        /*[in,out]*/ VARIANT_BOOL * GetNewValues,
        /*[out,retval]*/ VARIANT * pvarOut ) = 0;
      virtual HRESULT __stdcall RefreshData (
        /*[in,out]*/ long * TopicCount,
        /*[out,retval]*/ SAFEARRAY * * parrayOut ) = 0;
      virtual HRESULT __stdcall DisconnectData (
        /*[in]*/ long TopicID ) = 0;
      virtual HRESULT __stdcall Heartbeat (
        /*[out,retval]*/ long * pfRes ) = 0;
      virtual HRESULT __stdcall ServerTerminate ( ) = 0;
};

struct __declspec(uuid("a43788c1-d91b-11d3-8f39-00c04f3651b8"))
IRTDUpdateEvent : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall UpdateNotify ( ) = 0;
      virtual HRESULT __stdcall get_HeartbeatInterval (
        /*[out,retval]*/ long * plRetVal ) = 0;
      virtual HRESULT __stdcall put_HeartbeatInterval (
        /*[in]*/ long plRetVal ) = 0;
      virtual HRESULT __stdcall Disconnect ( ) = 0;
};
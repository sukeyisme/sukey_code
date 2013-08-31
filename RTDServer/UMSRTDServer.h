// UMSRTDServer.h : CUMSRTDServer 的声明

#pragma once
#include "resource.h"       // 主符号
#include "RTDServer_i.h"
#include "iList.h"
#include "../UMSPipe/UMSNamePipe.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

// CUMSRTDServer

class ATL_NO_VTABLE CUMSRTDServer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUMSRTDServer, &CLSID_UMSRTDServer>,
	public IDispatchImpl<IUMSRTDServer, &IID_IUMSRTDServer, &LIBID_RTDServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IRtdServer, &__uuidof(IRtdServer), &LIBID_Excel, /* wMajor = */ 1, /* wMinor = */ 6>
{
public:
	CUMSRTDServer();

	~CUMSRTDServer();


	DECLARE_REGISTRY_RESOURCEID(IDR_UMSRTDSERVER)

	DECLARE_NOT_AGGREGATABLE(CUMSRTDServer)

	BEGIN_COM_MAP(CUMSRTDServer)
		COM_INTERFACE_ENTRY(IUMSRTDServer)
		COM_INTERFACE_ENTRY2(IDispatch, IRtdServer)
		COM_INTERFACE_ENTRY(IRtdServer)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
public:
	STDMETHOD(ServerStart)(IRTDUpdateEvent * CallbackObject, long * pfRes);
	STDMETHOD(ConnectData)(long TopicID, SAFEARRAY * * Strings, VARIANT_BOOL * GetNewValues, VARIANT * pvarOut);
	STDMETHOD(RefreshData)(long * TopicCount, SAFEARRAY * * parrayOut);
	STDMETHOD(DisconnectData)(long TopicID);
	STDMETHOD(Heartbeat)(long * pfRes);
	STDMETHOD(ServerTerminate)();

	void StringToArray(SAFEARRAY * * Strings,char (*chrArray)[32]);
private:
	DWORD m_lDataThread;
	
	TList<long>* m_pTopicList;
	UMSNamePipeClient g_AddinNamePipe;
};
OBJECT_ENTRY_AUTO(__uuidof(UMSRTDServer), CUMSRTDServer)

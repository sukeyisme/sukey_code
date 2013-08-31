// UMSRTDServer.h : CUMSRTDServer ������

#pragma once
#include "resource.h"       // ������
#include "RTDServer_i.h"
#include "iList.h"
#include "../UMSPipe/UMSNamePipe.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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

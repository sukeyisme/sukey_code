// dllmain.h : 模块类的声明。
class CRTDServerModule : public CAtlDllModuleT< CRTDServerModule >
{
public :
	DECLARE_LIBID(LIBID_RTDServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_RTDSERVER, "{9D994025-A4CE-4EE4-AA65-D3A1A5FB7297}")
};

extern class CRTDServerModule _AtlModule;

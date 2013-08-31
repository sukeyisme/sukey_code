// UMSRTDServer.cpp : CUMSRTDServer µÄÊµÏÖ

#include "stdafx.h"
#include "UMSRTDServer.h"
#define RTDSERVERTRACE(x) OutputDebugString(x)
CUMSRTDServer::CUMSRTDServer()
{
	m_lDataThread = -1;	
	m_pTopicList = new TList<long>();
}

CUMSRTDServer::~CUMSRTDServer()
{
	 if (m_pTopicList != NULL)
	 {
      delete m_pTopicList;
	 }
}

HRESULT CUMSRTDServer::ServerStart(IRTDUpdateEvent * CallbackObject, long * pfRes /* <= 0 means failure */)
{
	RTDSERVERTRACE(_T("VCRTDServer::ServerStart\n"));
	 HRESULT hr = S_OK;
	if(CallbackObject == NULL || pfRes == NULL)
	{
		hr = E_POINTER;
	}
	else if(m_lDataThread!=-1)
	{
		hr = E_FAIL;
		*pfRes = -1;
	}
	else
	{
		IStream* pMarshalStream = NULL;
		hr = CoMarshalInterThreadInterfaceInStream(IID_IRTDUpdateEvent,CallbackObject,&pMarshalStream);
	CreateThread(NULL,0,RealTimeDataThread,pMarshalStream,0,&m_lDataThread);

	*pfRes = m_lDataThread;
	}
	return hr;
}

HRESULT CUMSRTDServer::ConnectData(long TopicID, SAFEARRAY * * Strings, VARIANT_BOOL * GetNewValues, VARIANT * pvarOut)
{
	RTDSERVERTRACE(_T("VCRTDServer::ConnectData\n"));
	HRESULT hr = S_OK;

	//g_AddinNamePipe.check_connect();

	//g_AddinNamePipe.SendCommand(1,512,_T("just test!"));
	g_AddinNamePipe.SetPipeName(_T("__YiYi_Dance__"));
	g_AddinNamePipe.Initialize();
	

	if(pvarOut==NULL)
		hr = E_POINTER;
	else
	{
		BOOL bFlag(TRUE);
	LONG lbound, ubound;
	SafeArrayGetLBound(*Strings,1,&lbound);
	SafeArrayGetUBound(*Strings,1,&ubound);
	
	VARIANT* pvar;
	SafeArrayAccessData(*Strings, (void HUGEP**) &pvar);
	for(long i = lbound; i <= ubound; i++)
	{
		BSTR bs = pvar[i].bstrVal;
		_bstr_t b = bs;
		char* temp = b;
		if(0==strcmp(temp,"Refresh"))
		{
			PostThreadMessage(m_lDataThread,WM_COMMAND,WM_REFRESH,0);
			bFlag = FALSE;
		}
	}
	SafeArrayUnaccessData(*Strings);
	
	if(bFlag)
		m_pTopicList->InsertAfter( TopicID );
	 SYSTEMTIME sysTime;
     GetSystemTime( &sysTime );

	 static WCHAR valBuffer[80];
	 //swprintf( valBuffer, L"TopicID: %d | %02d:%02d:%02d", TopicID,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	 swprintf( valBuffer, L"{1,2,3;10,20,30}");

	  VariantInit( pvarOut );
	  pvarOut->vt = VT_BSTR;
	  pvarOut->bstrVal = SysAllocString( valBuffer );
	}

	return S_OK;
}

HRESULT CUMSRTDServer::RefreshData(long * TopicCount, SAFEARRAY * * parrayOut)
{
	RTDSERVERTRACE(_T("VCRTDServer::RefreshData\n"));
	HRESULT hr = S_OK;

	if ((TopicCount == NULL) || (parrayOut == NULL) || (*parrayOut != NULL))
	{
      hr = E_POINTER;
      RTDSERVERTRACE(_T("   Bad pointer\n"));
    }
	else
	{
		*TopicCount = m_pTopicList->Size();;

		SAFEARRAYBOUND bounds[2];
      SYSTEMTIME sysTime;
      static WCHAR valBuffer[64];
      VARIANT value;
      long index[2];
      GetSystemTime( &sysTime );

      bounds[0].cElements = 2;
      bounds[0].lLbound = 0;
      bounds[1].cElements = m_pTopicList->Size();;
      bounds[1].lLbound = 0;
      *parrayOut = SafeArrayCreate(VT_VARIANT, 2, bounds);
      
         int size = m_pTopicList->Size(); 
      m_pTopicList->Reset(); 
      for (int i = 0; i < size; i++){ 
         index[0] = 0;
         index[1] = i;


		 
         VariantInit(&value);
         value.vt = VT_I4;
         value.lVal = m_pTopicList->Retrieve();
         SafeArrayPutElement( *parrayOut, index, &value);
         
         index[0] = 1;
         index[1] = i;

         /*swprintf( valBuffer, L"TopicID: %d | %02d:%02d:%02d", value.lVal,
            sysTime.wHour, sysTime.wMinute, sysTime.wSecond);*/
		 static BOOL bToggle;
		 bToggle = !bToggle;
		 if(bToggle)
			swprintf( valBuffer, L"{1,2,3;10,20,30}");
		 else
			swprintf( valBuffer, L"{\"a\",\"b\",\"c\";\"d\",\"e\",\"f\"}");

         VariantInit(&value);
         /*value.vt = VT_BSTR;
         value.bstrVal = SysAllocString( valBuffer );*/
		 value.vt = VT_BSTR;
		 value.bstrVal =SysAllocString(valBuffer);
         SafeArrayPutElement( *parrayOut, index, &value);

         (*m_pTopicList)++; 
	  }
	}
	
	return hr;
}

HRESULT CUMSRTDServer::DisconnectData(long TopicID)
{
	 //tracing purposes only
   RTDSERVERTRACE(_T("VCRTDServer::DisconnectData\n"));
   HRESULT hr = S_OK;

   //Search for the topic id and remove it
   if (m_pTopicList->Includes( TopicID ))
      m_pTopicList->Remove();

	return S_OK;
}

HRESULT CUMSRTDServer::Heartbeat(long * pfRes)
{
	HRESULT hr = S_OK;
	if(pfRes == NULL)
		hr = E_POINTER;
	else
		*pfRes = 1;
	return hr;
}

HRESULT CUMSRTDServer::ServerTerminate()
{
   RTDSERVERTRACE(_T("VCRTDServer::ServerTerminate\n"));
   HRESULT hr = S_OK;

   if (m_lDataThread != -1){
      PostThreadMessage( m_lDataThread, WM_COMMAND, WM_TERMINATE, 0 );
   }
	return hr;
}

void CUMSRTDServer::StringToArray(SAFEARRAY **Strings, char (*chrArray)[32])
{
	
}
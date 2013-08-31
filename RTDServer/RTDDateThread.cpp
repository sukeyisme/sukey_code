#include "stdafx.h"
#include "RTDDateThread.h"
#include "windowsx.h"
IRTDUpdateEvent* pRTDUpdate = NULL;

DWORD WINAPI RealTimeDataThread( LPVOID pMarshalStream)
{
	CoInitialize( NULL );
	DWORD dwRetVal = 0;
	 HRESULT hr = S_OK;

	 hr = CoGetInterfaceAndReleaseStream( (IStream*)pMarshalStream, 
      IID_IRTDUpdateEvent, (void**)&pRTDUpdate );

	if (pRTDUpdate != NULL){
      pRTDUpdate->AddRef();
      hr = pRTDUpdate->put_HeartbeatInterval( 1200 );
   
      UINT timerID = SetTimer( NULL, 0, 1000, NULL );

      dwRetVal = MessageLoop();
      
      KillTimer( NULL, timerID );

      pRTDUpdate->Release();
   }

   CoUninitialize();

	return dwRetVal;
}

WPARAM MessageLoop()
{
   MSG msg;
   HRESULT hr;

   while (GetMessage( &msg, NULL, 0, 0)){
      switch( msg.message ){
      case WM_TIMER:
         //hr = pRTDUpdate->UpdateNotify();
         break;
      case WM_COMMAND:
         HANDLE_WM_COMMAND(msg.hwnd, msg.wParam, msg.lParam, ThreadOnCommand);
         break;
      default:
         break;
      }
   }
   return msg.wParam;
}
void ThreadOnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
   HRESULT hr = S_OK;
   //switch on the command
   switch( id ){
   case WM_TERMINATE:
      hr = pRTDUpdate->Disconnect();
   case WM_SILENTTERMINATE:
      PostQuitMessage( hr );
      break;
   case WM_REFRESH:
	   hr = pRTDUpdate->UpdateNotify();
	   break;
   //case WM_GETSTOCKINFO:
	   //hr = pRTDUpdate->
   default:
      break;
   }
}
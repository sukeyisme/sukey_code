#include "stdafx.h"
extern HANDLE g_hInst;
HANDLE hArray;
HWND g_hWndMain = NULL;
XCHAR g_szBuffer[20] = L"";

XLOPER12 CallRTD( LPWSTR library, LPWSTR server, int numArgs, LPWSTR args[]) 
{
	
	int iStrings = -1;
	// 28 parameters max
	LPXLOPER12 lparameters[30] = {0};
	/*static */XLOPER12 result;	
	
	int li = 0;
	lparameters[li++] = (LPXLOPER12)TempStr12(library);
	lparameters[li++] = (LPXLOPER12)TempStr12(server);	


	for(int i = 0; i < numArgs; i++)
	{
		lparameters[li++] = (LPXLOPER12)TempStr12(args[i]);
	}

	int rc = 0;

	rc = Excel12v(xlfRtd, &result, li, (LPXLOPER12 *)lparameters);

	if(rc != xlretSuccess)
	{
		debugPrintf("Excel12v returned %d.\n", rc);
	}

	/* Free the the temp memoery */
	FreeAllTempMemory();
	result.xltype |= xlbitXLFree;
	return result;
} 

VOID RegisterHelper(struct XLLRegisterInfo registryInfo)
{
    static XLOPER12 xDLL, xRegId;
    static LPXLOPER12 args[260];
    int count;
    int i;
    
    
    count = 10 + registryInfo.argumentCount;

    /*
        Get the name of the DLL.
    */
    Excel12f(xlGetName, &xDLL, 0);
    

    if(registryInfo.argumentCount == 0)
    {
        Excel12f(xlfRegister, 0, count,
            (LPXLOPER12)&xDLL, /* moduleText */
            (LPXLOPER12)TempStr12(registryInfo.procedure),
            (LPXLOPER12)TempStr12(registryInfo.typeText),
            (LPXLOPER12)TempStr12(registryInfo.functionText),
            (LPXLOPER12)TempStr12(registryInfo.argumentText),
            (LPXLOPER12)TempInt12(registryInfo.macroType), /* function listed in function wizard */
            (LPXLOPER12)TempStr12(registryInfo.categoryName),
            (LPXLOPER12)TempStr12(L""), /* shortcut text */
            (LPXLOPER12)TempStr12(registryInfo.helpTopic),
            (LPXLOPER12)TempStr12(registryInfo.functionHelp));
    }
    else
    {
        count = 0;
        args[count++] = (LPXLOPER12)&xDLL; /* moduleText */
        args[count++] = (LPXLOPER12)TempStr12(registryInfo.procedure);
        args[count++] =    (LPXLOPER12)TempStr12(registryInfo.typeText);
        args[count++] = (LPXLOPER12)TempStr12(registryInfo.functionText);
        args[count++] = (LPXLOPER12)TempStr12(registryInfo.argumentText);
        args[count++] =    (LPXLOPER12)TempInt12(registryInfo.macroType); /* function listed in function wizard */
        args[count++] = (LPXLOPER12)TempStr12(registryInfo.categoryName);
        args[count++] =    (LPXLOPER12)TempStr12(L""); /* shortcut text */
        args[count++] =    (LPXLOPER12)TempStr12(registryInfo.helpTopic);
        args[count++] = (LPXLOPER12)TempStr12(registryInfo.functionHelp);
        
        for(i = 0; i < registryInfo.argumentCount; i++)
        {
            args[count++] = (LPXLOPER12)TempStr12(registryInfo.argumentHelp[i]);
        }
 
        Excel12v(xlfRegister, 0, count,
            args);
    }
}

struct XLLRegisterInfo rgFuncs[] = 
{ 
    {
        L"GetStockPrice" /* procedure */, L"UU" /*typeText*/, /* functionText */ L"GetStockPrice", 
        /*argumentText*/ L"", 1, L"Examples", L"" /*shortcutText*/, L"" /*helpTopic*/, 
        L"sum of all." /* functionHelp */, 1 /*argCount */,
        {L"nothing. " /*argumentHelp1 pad with space, Excel bug*/}
    },
	{
		L"GetStockInfo",L"UU",L"如果",
			L"",1,L"Examples",L"",L"",
			L"Stock Info",1,
		{L"nothing"}
	},
	{
		L"SelfServiceReport",L"",L"SelfServiceReport",
			L"",1,L"Examples",L"",L"",
			L"Report",1,
		{L"nothing"}
	},
    { NULL }
};


__declspec(dllexport) int WINAPI xlAutoOpen()
{
	for(int i=0;rgFuncs[i].procedure != NULL;i++)
		RegisterHelper(rgFuncs[i]);
	
	return 1;
}

__declspec(dllexport) LPXLOPER12 WINAPI xlAddInManagerInfo12(LPXLOPER12 xAction)
{
	static XLOPER12 xInfo, xIntAction;

	//
	// This code coerces the passed-in value to an integer. This is how the
	// code determines what is being requested. If it receives a 1, 
	// it returns a string representing the long name. If it receives 
	// anything else, it returns a #VALUE! error.
	//

	Excel12f(xlCoerce, &xIntAction, 2, xAction, TempInt12(xltypeInt));

	if (xIntAction.val.w == 1)
	{
		xInfo.xltype = xltypeStr;
		xInfo.val.str = L"\022UMSAddin DLL";
	}
	else
	{
		xInfo.xltype = xltypeErr;
		xInfo.val.err = xlerrValue;
	}

	//Word of caution - returning static XLOPERs/XLOPER12s is not thread safe
	//for UDFs declared as thread safe, use alternate memory allocation mechanisms
	return(LPXLOPER12) &xInfo;
}

__declspec(dllexport) int WINAPI xlAutoRemove(void)
{
	// Show a dialog box indicating that the XLL was successfully removed //
	Excel12f(xlcAlert, 0, 2, TempStr12(L"Thank you for removing UMSAddin.XLL!"),
		  TempInt12(2));
	return 1;
}

__declspec(dllexport) int WINAPI xlAutoAdd(void)
{
	XCHAR szBuf[255];

	wsprintfW((LPWSTR)szBuf, L"Thank you for adding UMSAddin.XLL\n "
			 L"built on %hs at %hs", __DATE__, __TIME__);

	// Display a dialog box indicating that the XLL was successfully added //
	Excel12f(xlcAlert, 0, 2, TempStr12(szBuf), TempInt12(2));
	return 1;
}
__declspec(dllexport) int WINAPI xlAutoClose(void)
{
	XLOPER12 xRes;

	//
	// This block first deletes all names added by xlAutoOpen or
	// xlAutoRegister12. Next, it checks if the drop-down menu Generic still
	// exists. If it does, it is deleted using xlfDeleteMenu. It then checks
	// if the Test toolbar still exists. If it is, xlfDeleteToolbar is
	// used to delete it.
	//

	//
	// Due to a bug in Excel the following code to delete the defined names
	// does not work.  There is no way to delete these
	// names once they are Registered
	// The code is left in, in hopes that it will be
	// fixed in a future version.
	//
		for(int i=0;rgFuncs[i].procedure != NULL;i++)
			Excel12f(xlfSetName, 0, 1, TempStr12(rgFuncs[i].functionText));
	//
	// Everything else works as documented
	//

	return 1;
}

//自定义
__declspec(dllexport) LPXLOPER12 WINAPI GetStockPrice(LPXLOPER12 px)
{
	LPWSTR args[1]={0};
	WCHAR temp[16] = {0};
	
	wsprintf(temp,_T("%d"),(long)px->val.num);
	args[0] = temp;
	static XLOPER12 result = CallRTD(L"rtdserver.umsrtdserver.1", L"", 1, args);
	return (LPXLOPER12) &result;
}

__declspec(dllexport) int WINAPI SelfServiceReport()
{
	LPWSTR args[1]={0};
	args[0] = _T("Refresh");
	
	XLOPER12 result = CallRTD(L"rtdserver.umsrtdserver", L"", 1, args);
	//XLOPER12 xRef;
	//Excel12(xlfSelection,&xRef,0);
	//int boolSheet;
	//boolSheet = (Excel12f(xlSheetId, 0, 0) == xlretSuccess);
	//if(boolSheet)
	//{
	//	LPXLOPER12 pxArray;
	//	static XLOPER12 xMulti;
	//	static __int64 x=0; 
	//	__int64 i;
	//	__int64 rwcol;

	//	xMulti.xltype = xltypeMulti | xlbitDLLFree;
	//	xMulti.val.array.columns = 3;
	//	xMulti.val.array.rows = /*10*/3;

	//	rwcol = (__int64)xMulti.val.array.columns * xMulti.val.array.rows; 
	//	//hArray = GlobalAlloc(GMEM_ZEROINIT,542*1024*1024);
	//	pxArray = (LPXLOPER12)GlobalLock(hArray = GlobalAlloc(GMEM_ZEROINIT,sizeof(XLOPER12)*rwcol));
	//	//pxArray = (LPXLOPER12)new char(rwcol*sizeof(XLOPER12));
	//	//long c = sizeof(XLOPER12);
	//	
	//	//pxArray = new XLOPER12[64*1024*1024-1];
	//	
	//	xMulti.val.array.lparray = pxArray;

	//	for(i = 0; i < rwcol; i++) 
	//	{
	//		pxArray[i].xltype = xltypeInt;
	//		pxArray[i].val.w = /*L"\013=如果(RC[-1])"*/99;
	//	}

	int a = Excel12(xlSet, 0, 2, TempActiveRef12(0,2,0,2), (LPXLOPER12)&result);
	//	//Excel12f(xlcFormula, 0, 2, (LPXLOPER12)&xMulti, TempActiveRef12(3,5,3,5));
	//	int b = 0;
	//}

	return 99;
}




__declspec(dllexport) LPXLOPER12 WINAPI GetStockInfo(LPXLOPER12 px)
{
	LPXLOPER12 pxArray;
	XLOPER12 res;
	int rwcol;

	res.xltype = xltypeMulti;
	res.val.array.columns = 3;
	res.val.array.rows = 2;

	rwcol = res.val.array.columns * res.val.array.rows;
	pxArray = (LPXLOPER12)GlobalLock(hArray = GlobalAlloc(GMEM_ZEROINIT, rwcol * sizeof(XLOPER12)));

	res.val.array.lparray = pxArray;
	for(long i = 0; i < rwcol; i++) 
	{
		pxArray[i].xltype = xltypeInt;
		pxArray[i].val.w = i;
	}

	return (LPXLOPER12)&res;
}

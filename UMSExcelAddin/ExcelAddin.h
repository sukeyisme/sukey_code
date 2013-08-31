#include "xlcall.h"
//����xll��excel�Ľӿں���
//int	__stdcall xlAutoOpen(void);//��ʼ��xll����ע�ắ��,������
//int	__stdcall xlAutoAdd(void);
//int	__stdcall xlAutoClose(void);//�ر�xll
//int	__stdcall xlAutoRemove(void);
//LPXLOPER __stdcall	xlAddInManagerInfo(LPXLOPER xAction);
//void	__stdcall xlAutoFree(XLOPER *pXloper);

struct XLLRegisterInfo
{
    /**
        Name of the procedure to be registered.
    */
    LPWSTR procedure;
    /** 
        The type text see pxTypeText here: http://msdn.microsoft.com/en-us/library/bb687900.aspx
    */
    LPWSTR typeText;
    /**
        Name of the function in Excel.
    */
    LPWSTR functionText;
    /**
        String of Letters that represent arguments/types. See pxArgumentText
        here: http://msdn.microsoft.com/en-us/library/bb687900.aspx
    */
    LPWSTR argumentText;
    /**
        Type of function. 0 - Macro sheet equivalent, 1 - Worksheet Functions, 2 - Commands.
        When in doubt use 1;
     */
    int macroType;

    /**
        Name of the category to add the function to.
    */
    LPWSTR categoryName;

    /**
        Commands only.
    */
    LPWSTR shortcutText;

    /**
        Reference to a help file (.chm or .hlp). 
        Use form: filepath!HelpContextID or http://address/path_to_file_in_site!0
    */
    LPWSTR helpTopic;
    /**
        String that describes the function in the function wizard.
    */
    LPWSTR functionHelp;

    /**
      * Number of descriptions in argumentHelp.
      */
    int argumentCount;

    /**
        Array of Argument help strings. 
    */
    LPWSTR argumentHelp[15];
};

BOOL CALLBACK DIALOGMsgProc(HWND hWndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL GetHwnd(HWND * pHwnd);

#define FREE_SPACE                  104

VOID RegisterHelper(struct XLLRegisterInfo registryInfo);//ע���������

//�Զ��庯��
__declspec(dllexport) LPXLOPER12 WINAPI GetStockPrice(LPXLOPER12 px);
__declspec(dllexport) int WINAPI SelfServiceReport();
__declspec(dllexport) LPXLOPER12 WINAPI GetStockInfo(LPXLOPER12 px);
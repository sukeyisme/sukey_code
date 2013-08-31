using System;
using UMSAddinUI.Properties;
using Microsoft.Office.Core;
using Microsoft.Office.Interop.Excel;
using Microsoft.Win32;
using System.Runtime.InteropServices;
using Extensibility;
using WinForms = System.Windows.Forms;

namespace UMSAddinUI
{
    [Guid(UMSAddinUI.TypeGuid)]
    [ProgId(UMSAddinUI.TypeProgId)]
    [ClassInterface(ClassInterfaceType.AutoDual)]
    public sealed class UMSAddinUI : IRibbonExtensibility,IDTExtensibility2
    {
        [DllImport("C:\\Users\\sukey\\Desktop\\sukey-code\\Debug\\UMSExcelAddin.xll",
            EntryPoint = "RefreshData",
            CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern int RefreshData();
 

        public const string TypeGuid = "02EF4BAA-7CDF-4030-8109-A9B73EF73161";
        public const string TypeProgId = "UMSAddinUI";
        public const string ClsIdKeyName = @"CLSID\{" + UMSAddinUI.TypeGuid + @"}\";
        public const string ExcelAddInKeyName = @"Software\Microsoft\Office\Excel\Addins\" + UMSAddinUI.TypeProgId;
        Application _excel;

        public UMSAddinUI()
        { 
        
        }

        [ComRegisterFunctionAttribute]
        public static void RegisterFunction(Type t)
        {
            RegistryKey key;

            key = Registry.ClassesRoot.CreateSubKey(UMSAddinUI.ClsIdKeyName + "Programmable");
            key.Close();

            key = Registry.ClassesRoot.CreateSubKey(UMSAddinUI.ClsIdKeyName + "InprocServer32");
            key.SetValue(string.Empty, Environment.SystemDirectory + @"\mscoree.dll");
            key.Close();

            //key = Registry.CurrentUser.CreateSubKey(UserInterface.ExcelAddInKeyName);
            key = Registry.LocalMachine.CreateSubKey(UMSAddinUI.ExcelAddInKeyName);
            key.SetValue("Description", "Parago.de Stock Quotes UI Add-In for Excel 2007", RegistryValueKind.String);
            key.SetValue("FriendlyName", "Parago.de Stock Quotes UI Add-In", RegistryValueKind.String);
            key.SetValue("LoadBehavior", 3, RegistryValueKind.DWord);
            key.SetValue("CommandLineSafe", 0, RegistryValueKind.DWord);
            key.Close();
        }

        [ComUnregisterFunctionAttribute]
        public static void UnregisterFunction(Type t)
        {
            Registry.ClassesRoot.DeleteSubKey(UMSAddinUI.ClsIdKeyName + "Programmable");
            Registry.ClassesRoot.DeleteSubKeyTree(UMSAddinUI.ClsIdKeyName + "InprocServer32");
            Registry.LocalMachine.DeleteSubKey(UMSAddinUI.ExcelAddInKeyName);
        }

        public string GetCustomUI(string ribbonID)
        {
            return Resources.Ribbon;
        }

        public void OnUpdateButtonClick(IRibbonControl control)
        { 
            RefreshData();
        }

        public void OnAboutButtonClick(IRibbonControl control)
        { 
            
        }

        public void OnBeginShutdown(ref Array custom)
        {

        }

        public void OnStartupComplete(ref Array custom)
        {
        }

        public void OnAddInsUpdate(ref Array custom)
        {
        }

        public void OnConnection(object host, ext_ConnectMode connectMode, object addInInst, ref Array custom)
        {
            _excel = (Application)host;
        }

        public void OnDisconnection(ext_DisconnectMode removeMode, ref Array custom)
        {
        }
    }
}

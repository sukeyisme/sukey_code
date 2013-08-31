namespace UMSExcelAddinUI
{
	using System;
	using Extensibility;
	using System.Runtime.InteropServices;
    using Microsoft.Office.Core;
    using Excel = Microsoft.Office.Interop.Excel;
    using System.Reflection;
    using UMSExcelAddinUI.Properties;
    using System.Drawing.Imaging;
    using System.Drawing;
    using System.IO;
    using System.Runtime.InteropServices.ComTypes;
    using Office = Microsoft.Office.Core;
    using System.Windows.Forms;
    using System.IO.Pipes;
    using System.Text;
    using System.Collections;
    using Microsoft.Win32;

    public class ConvertImage : System.Windows.Forms.AxHost
    {
        protected ConvertImage(string clsid)
            : base(clsid)
        {
            
        }
        public static stdole.IPictureDisp Convert2Picture(Image img)
        {
            return (stdole.IPictureDisp)GetIPictureDispFromPicture(img);
        }
    }
    
        //public static extern int RefreshData();

    enum EXCEL_PROTOCOL_ID
    {
        ONLY_HEAD = 9,
        REPORT_TYPE,
        REPORT_INDEX,
        REPORT_STOCK
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ums_rpttype
    {
        public UInt32 TYPEIDX;
        public UInt32 RPTTYPE;
        public UInt32 RPTMASK;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string IDXLIST;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string RPTNAME;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 201)]
        public string SCRIPT;
        public ums_rpttype(string s)
        {
            TYPEIDX = 0;
            RPTTYPE=0;
            RPTMASK=0;
            IDXLIST =s;
            RPTNAME =s;
            SCRIPT=s;

        }
    }
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ums_rptfunc
    {
        public UInt32 FUNCKEY;
        public UInt32 RPTTYPE;
        public UInt32 PARENTID;
        public UInt32 FUNCID;
        public UInt32 ISGROUP;
        public UInt32 ICONIDX;
        public UInt32 func;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string FUNCNAME;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 201)]
        public string SCRIPT;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
        public string PARAM;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 4096)]
        public string SOURCE;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 4096)]
        public string VARPARAM;

        public ums_rptfunc(string s)
        {
            FUNCKEY = 0;
            RPTTYPE = 0;
            PARENTID = 0;
            FUNCID = 0;
            ISGROUP = 0;
            ICONIDX = 0;
            func = 0;
            FUNCNAME = s;
            SCRIPT = s;
            PARAM = s;
            SOURCE = s;
            VARPARAM = s;
        }
    }
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct ums_block_index
    {
        public byte Leverl;
        public byte Unkown;
        public byte SamePCode;
        public UInt32 bkType;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string pszCode;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
        public string szCode;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public string szName;
        public ums_block_index(string s)
        {
            Leverl = 0;
            Unkown = 0;
            SamePCode = 0;
            bkType = 0;
            pszCode = s;
            szCode = s;
            szName = s;
        }
    }
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct my_header
    {
        public UInt32 _lCommandID;
        public UInt32 _lPackSize;
        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = 3)]
        public UInt32[] _lExtendData;
        public my_header(string s)
        {
            _lCommandID = 0;
            _lPackSize = 0;
            _lExtendData = new UInt32[] { 0, 0, 0 };
        }
    }

	#region Read me for Add-in installation and setup information.
	// When run, the Add-in wizard prepared the registry for the Add-in.
	// At a later time, if the Add-in becomes unavailable for reasons such as:
	//   1) You moved this project to a computer other than which is was originally created on.
	//   2) You chose 'Yes' when presented with a message asking if you wish to remove the Add-in.
	//   3) Registry corruption.
	// you will need to re-register the Add-in by building the UMSExcelAddinUISetup project, 
	// right click the project in the Solution Explorer, then choose install.
	#endregion
	
	/// <summary>
	///   The object for implementing an Add-in.
	/// </summary>
	/// <seealso class='IDTExtensibility2' />
	[GuidAttribute("2521708C-66C6-4C50-A89E-D13DB01C7C9C"), ProgId("UMSExcelAddinUI.Connect")]
	public class Connect : Object, Extensibility.IDTExtensibility2,IRibbonExtensibility
	{
		/// <summary>
		///		Implements the constructor for the Add-in object.
		///		Place your initialization code within this method.
		/// </summary>
        /// 

        [DllImport("RTDServer.dll")]
        public static extern int DllRegisterServer();//注册时用

        //excel 2003 限制：行65536列256！2007：行1048576列16384
        public const Int32 _MAX_2007_ROW_ = 1048576;
        public const Int32 _MAX_2007_COL_ = 16384;
        public const Int32 _MAX_2003_ROW_ = 65536;
        public const Int32 _MAX_2003_COL_ = 256;
        public Int32 ExcelVersion = 2007;

		public Connect()
		{
            //Excel.Application excelTemp = new Excel.ApplicationClass();

            //excelTemp.Workbooks.Add(true);

            //Excel.AddIn xllAddin = excelTemp.AddIns.Add("C:\\Users\\sukey\\Desktop\\sukey-code\\Debug\\UMSExcelAddin.xll", true);
            //xllAddin.Installed = true;
            //excelTemp.Quit();
            //excelTemp = null;
		}
        private Excel.Application applicationObject;

		/// <summary>
		///      Implements the OnConnection method of the IDTExtensibility2 interface.
		///      Receives notification that the Add-in is being loaded.
		/// </summary>
		/// <param term='application'>
		///      Root object of the host application.
		/// </param>
		/// <param term='connectMode'>
		///      Describes how the Add-in is being loaded.
		/// </param>
		/// <param term='addInInst'>
		///      Object representing this Add-in.
		/// </param>
		/// <seealso class='IDTExtensibility2' />
        /// 
        private string GetAssemblyPath()
        {
            string _CodeBase = Assembly.GetExecutingAssembly().CodeBase;
            _CodeBase = _CodeBase.Substring(8, _CodeBase.Length - 8);    // 8是 file:// 的长度
            string[] arrSection = _CodeBase.Split(new char[] { '/' });

            string _FolderPath = "";
            for (int i = 0; i < arrSection.Length - 1; i++)
            {
                _FolderPath += arrSection[i] + "/";
            }
            return _FolderPath;
        }
 
        private static string GetSubKeyName(Type type,string subKeyName)
        {
 
            System.Text.StringBuilder s = new System.Text.StringBuilder();
            s.Append(@"CLSID\{");
            s.Append(type.GUID.ToString().ToUpper());
            s.Append(@"}\");
            s.Append(subKeyName);
 
            return s.ToString();
        }

		public void OnConnection(object application, Extensibility.ext_ConnectMode connectMode, object addInInst, ref System.Array custom)
		{
            applicationObject = (Excel.Application)application;
            addInInstance = addInInst;

            //注册xll
            string path = GetAssemblyPath();
            string xllPath = path + "UMSExcelAddin.xll";
            //注册xll
            applicationObject.RegisterXLL(xllPath);
            //注册rtd4
            RegistryKey rkTest = Registry.ClassesRoot.OpenSubKey("CLSID\\{449546C3-2F5A-434D-9AB7-12C9DE98EA6F}\\");
            if (rkTest == null)
            {
                DllRegisterServer();
                //Dll没有注册，在这里调用DllRegisterServer()吧
            }
            //applicationObject.get_Range().Calculate();
            Object oMissing = System.Reflection.Missing.Value;
          if (connectMode == ext_ConnectMode.ext_cm_Startup)
          {
            int a = string.Compare("11.0", applicationObject.Version);
            if (a >= 0)
            {
               ExcelVersion = 2003;
               CommandBar cmdBarSystem = applicationObject.CommandBars["Worksheet Menu Bar"];

               CommandBarControl cmdControlAddin = cmdBarSystem.Controls.Add(MsoControlType.msoControlPopup, oMissing, "", 11, true);
               cmdControlAddin.Caption = "UMSExcel Addin";

               CommandBarPopup cmdBarPopAddin = (CommandBarPopup)cmdControlAddin;
               CommandBar cmdBarAddin = cmdBarPopAddin.CommandBar;
               AddCommandBar(cmdBarAddin);

               //create a new bar
               CommandBar UMSBar = applicationObject.CommandBars.Add("UMSExcel Addin", MsoBarPosition.msoBarTop, false, true);
               UMSBar.Visible = true;
               //AddCommandBar(UMSBar);
            }
            else {
               ExcelVersion = 2007;
            }
           }
        }

        public void AddCommandBar(CommandBar ComBar)
        {
            CommandBarButton buttonInsertFormual = (CommandBarButton)ComBar.Controls.Add(MsoControlType.msoControlButton, 1, "", 1, true);
            buttonInsertFormual.Caption = "插入函数";
            buttonInsertFormual.Picture = ConvertImage.Convert2Picture(Resources.insert);
            buttonInsertFormual.Mask = ConvertImage.Convert2Picture(Resources.insert2);
            buttonInsertFormual.Style = MsoButtonStyle.msoButtonIconAndCaption;
            buttonInsertFormual.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonInsertFormulaClick);

            CommandBarButton buttonHistory = (CommandBarButton)ComBar.Controls.Add(MsoControlType.msoControlButton, 1, "", 2, true);
            buttonHistory.Caption = "历史行情";
            buttonHistory.Picture = ConvertImage.Convert2Picture(Resources.history);
            buttonHistory.Mask = ConvertImage.Convert2Picture(Resources.history2);
            buttonHistory.Style = MsoButtonStyle.msoButtonIconAndCaption;
            buttonHistory.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonHistoryClick);

            CommandBarButton buttonRealTime = (CommandBarButton)ComBar.Controls.Add(MsoControlType.msoControlButton, 1, "", 3, true);
            buttonRealTime.Caption = "实时行情";
            buttonRealTime.Picture = ConvertImage.Convert2Picture(Resources.realtime);
            buttonRealTime.Mask = ConvertImage.Convert2Picture(Resources.realtime2);
            buttonRealTime.Style = MsoButtonStyle.msoButtonIconAndCaption;
            buttonRealTime.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonRealTimeClick);

            CommandBarPopup cmdBarPopReport = (CommandBarPopup)ComBar.Controls.Add(MsoControlType.msoControlPopup, 1, "", 4, true);
            cmdBarPopReport.Caption = "截面报表";
            CommandBar cmdBarReport = cmdBarPopReport.CommandBar;

            CommandBarButton buttonWizzard = (CommandBarButton)cmdBarReport.Controls.Add(MsoControlType.msoControlButton, 1, "", 1, true);
            buttonWizzard.Caption = "向导式";
            buttonWizzard.Picture = ConvertImage.Convert2Picture(Resources.wizzard);
            buttonWizzard.Mask = ConvertImage.Convert2Picture(Resources.wizzard2);
            buttonWizzard.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonWizzardClick);

            CommandBarButton buttonLR = (CommandBarButton)cmdBarReport.Controls.Add(MsoControlType.msoControlButton, 1, "", 2, true);
            buttonLR.Caption = "左右式";
            buttonLR.Picture = ConvertImage.Convert2Picture(Resources.lr);
            buttonLR.Mask = ConvertImage.Convert2Picture(Resources.lr2);
            buttonLR.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonLRClick);

            CommandBarPopup cmdBarPopSnapshot = (CommandBarPopup)ComBar.Controls.Add(MsoControlType.msoControlPopup, 1, "", 5, true);
            cmdBarPopSnapshot.Caption = "建立快照";
            CommandBar cmdBarSnapshot = cmdBarPopSnapshot.CommandBar;

            CommandBarButton buttonPage = (CommandBarButton)cmdBarSnapshot.Controls.Add(MsoControlType.msoControlButton, 1, "", 1, true);
            buttonPage.Caption = "工作表快照";
            buttonPage.Picture = ConvertImage.Convert2Picture(Resources.shot);
            buttonPage.Mask = ConvertImage.Convert2Picture(Resources.shot2);
            buttonPage.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonSnapshotPageClick);

            CommandBarButton buttonFile = (CommandBarButton)cmdBarSnapshot.Controls.Add(MsoControlType.msoControlButton, 1, "", 2, true);
            buttonFile.Caption = "工作薄快照";
            buttonFile.Picture = ConvertImage.Convert2Picture(Resources.file);
            buttonFile.Mask = ConvertImage.Convert2Picture(Resources.file2);
            buttonFile.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonSnapshotFileClick);

            CommandBarPopup cmdBarPopRefresh = (CommandBarPopup)ComBar.Controls.Add(MsoControlType.msoControlPopup, 1, "", 6, true);
            cmdBarPopRefresh.Caption = "刷新所有工作薄";
            CommandBar cmdBarRefresh = cmdBarPopRefresh.CommandBar;

            CommandBarButton buttonRefreshSelected = (CommandBarButton)cmdBarRefresh.Controls.Add(MsoControlType.msoControlButton, 1, "", 1, true);
            buttonRefreshSelected.Caption = "刷新选中区域";
            buttonRefreshSelected.Picture = ConvertImage.Convert2Picture(Resources.selceted);
            buttonRefreshSelected.Mask = ConvertImage.Convert2Picture(Resources.selected2);
            buttonRefreshSelected.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonRefreshSelectedClick);

            CommandBarButton buttonRefreshPage = (CommandBarButton)cmdBarRefresh.Controls.Add(MsoControlType.msoControlButton, 1, "", 2, true);
            buttonRefreshPage.Caption = "刷新本页";
            buttonRefreshPage.Picture = ConvertImage.Convert2Picture(Resources.page);
            buttonRefreshPage.Mask = ConvertImage.Convert2Picture(Resources.page2);
            buttonRefreshPage.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonSnapshotPageClick);

            CommandBarButton buttonRefreshFile = (CommandBarButton)cmdBarRefresh.Controls.Add(MsoControlType.msoControlButton, 1, "", 3, true);
            buttonRefreshFile.Caption = "刷新整个文件";
            buttonRefreshFile.Picture = ConvertImage.Convert2Picture(Resources.refresh);
            buttonRefreshFile.Mask = ConvertImage.Convert2Picture(Resources.refresh2);
            buttonRefreshFile.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSButtonSnapshotFileClick);

            CommandBarButton buttonHelp = (CommandBarButton)ComBar.Controls.Add(MsoControlType.msoControlButton, 1, "", 7, true);
            buttonHelp.Caption = "使用手册";
            buttonHelp.Picture = ConvertImage.Convert2Picture(Resources.help);
            buttonHelp.Mask = ConvertImage.Convert2Picture(Resources.help2);
            buttonHelp.Style = MsoButtonStyle.msoButtonIconAndCaption;
            buttonHelp.Click += new _CommandBarButtonEvents_ClickEventHandler(OnUMSHelpClick);
        
        }

		/// <summary>
		///     Implements the OnDisconnection method of the IDTExtensibility2 interface.
		///     Receives notification that the Add-in is being unloaded.
		/// </summary>
		/// <param term='disconnectMode'>
		///      Describes how the Add-in is being unloaded.
		/// </param>
		/// <param term='custom'>
		///      Array of parameters that are host application specific.
		/// </param>
		/// <seealso class='IDTExtensibility2' />
		public void OnDisconnection(Extensibility.ext_DisconnectMode disconnectMode, ref System.Array custom)
		{
            long a = 0;
		}

		/// <summary>
		///      Implements the OnAddInsUpdate method of the IDTExtensibility2 interface.
		///      Receives notification that the collection of Add-ins has changed.
		/// </summary>
		/// <param term='custom'>
		///      Array of parameters that are host application specific.
		/// </param>
		/// <seealso class='IDTExtensibility2' />
		public void OnAddInsUpdate(ref System.Array custom)
		{
            long a = 0;
		}

		/// <summary>
		///      Implements the OnStartupComplete method of the IDTExtensibility2 interface.
		///      Receives notification that the host application has completed loading.
		/// </summary>
		/// <param term='custom'>
		///      Array of parameters that are host application specific.
		/// </param>
		/// <seealso class='IDTExtensibility2' />
		public void OnStartupComplete(ref System.Array custom)
		{
           
		}

		/// <summary>
		///      Implements the OnBeginShutdown method of the IDTExtensibility2 interface.
		///      Receives notification that the host application is being unloaded.
		/// </summary>
		/// <param term='custom'>
		///      Array of parameters that are host application specific.
		/// </param>
		/// <seealso class='IDTExtensibility2' />
		public void OnBeginShutdown(ref System.Array custom)
		{
            long a = 0;
		}
		
		//private object applicationObject;
		private object addInInstance;

        #region IRibbonExtensibility 成员

        public Object BytesToStruct(byte[] bytes, Type strcutType)
        {
            Int32 size = Marshal.SizeOf(strcutType);
            IntPtr buffer = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.Copy(bytes, 0, buffer, size);
                return Marshal.PtrToStructure(buffer, strcutType);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        public byte[] StructToBytes(Object structure)
        {
            Int32 size = Marshal.SizeOf(structure);
            Console.WriteLine(size);
            IntPtr buffer = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(structure, buffer, false);
                Byte[] bytes = new Byte[size];
                Marshal.Copy(buffer, bytes, 0, size);
                return bytes;
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        ArrayList rpttypes = new ArrayList();
        ArrayList rptfuncs = new ArrayList();
        ArrayList rptBlocks = new ArrayList();

        public void ReadQuota()
        {
            NamedPipeClientStream ClientPipe = new NamedPipeClientStream(".", "__YiYi_Dance__", PipeDirection.InOut);
            ClientPipe.Connect();

            my_header myheader = new my_header();
            myheader._lCommandID = (UInt32)EXCEL_PROTOCOL_ID.ONLY_HEAD;
            myheader._lPackSize = 0;
            myheader._lExtendData = new UInt32[] { (UInt32)EXCEL_PROTOCOL_ID.REPORT_INDEX, 0, 0 };

            byte[] buffer = StructToBytes(myheader);
            ClientPipe.Write(buffer, 0, buffer.Length);

            int bytesize = Marshal.SizeOf(typeof(my_header));
            byte[] headerbyte = new byte[bytesize];
            int readCount = 0;
            my_header myheaderrecv = new my_header();
            if ((readCount = ClientPipe.Read(headerbyte, 0, bytesize)) > 0)
            {
                myheaderrecv = (my_header)BytesToStruct(headerbyte, myheaderrecv.GetType());
            }
            bytesize = Marshal.SizeOf(typeof(ums_rptfunc));
            byte[] datebyte = new byte[bytesize];
            while (true)
            {
                if (rptfuncs.Count == myheaderrecv._lExtendData[0])
                    break;
                readCount = ClientPipe.Read(datebyte, 0, bytesize);
                if (readCount < 0) break;
                ums_rptfunc functemp = new ums_rptfunc();
                functemp = (ums_rptfunc)BytesToStruct(datebyte, functemp.GetType());
                rptfuncs.Add(functemp);
            }
            rptfuncs.TrimToSize();
            ClientPipe.Close();
        }

        public void ReadReportType()
        {
            NamedPipeClientStream ClientPipe = new NamedPipeClientStream(".", "__YiYi_Dance__", PipeDirection.InOut);
            ClientPipe.Connect();

            my_header myheader = new my_header();
            myheader._lCommandID = (UInt32)EXCEL_PROTOCOL_ID.ONLY_HEAD;
            myheader._lPackSize = 0;
            myheader._lExtendData = new UInt32[] { (UInt32)EXCEL_PROTOCOL_ID.REPORT_TYPE, 0, 0 };

            byte[] buffer = StructToBytes(myheader);
            ClientPipe.Write(buffer, 0, buffer.Length);

            int bytesize = Marshal.SizeOf(typeof(my_header));
            byte[] headerbyte = new byte[bytesize];
            int readCount = 0;
            my_header myheaderrecv = new my_header();
            if ((readCount = ClientPipe.Read(headerbyte, 0, bytesize)) > 0)
            {
                myheaderrecv = (my_header)BytesToStruct(headerbyte, myheaderrecv.GetType());
            }
            bytesize = Marshal.SizeOf(typeof(ums_rpttype));
            byte[] datebyte = new byte[bytesize];
            while (true)
            {
                if (rpttypes.Count == myheaderrecv._lExtendData[0])
                    break;
                readCount = ClientPipe.Read(datebyte, 0, bytesize);
                if (readCount < 0) break;
                ums_rpttype functemp = new ums_rpttype();
                functemp = (ums_rpttype)BytesToStruct(datebyte, functemp.GetType());
                rpttypes.Add(functemp);
            }
            rpttypes.TrimToSize();
            ClientPipe.Close();
        }

        public void OnWizzard()
        { 
            //RefreshData();
            //applicationObject.Evaluate("GetStockInfo(1)");
            //Object objectReference = "RTDServer.UMSRTDServer.1";
            //applicationObject.COMAddIns.Item(ref objectReference).Object;
            Object ob = applicationObject.ExecuteExcel4Macro("SelfServiceReport()");

            ReadQuota();    //从服务器读取界面报表指标
            ReadReportType();
            FormBase FormTree = new FormBase();
            FormTree.NodeList = rptfuncs;
            WizardController controller = new WizardController(FormTree);
            controller.formbase.InitQuotaTree(11, 0,null);   //将指标显示在Tree控件中
            controller.BeginWizard();           
        }

        public void OnLR()
        {
            NamedPipeClientStream ClientPipe = new NamedPipeClientStream(".", "__YiYi_Dance__", PipeDirection.InOut);
            ClientPipe.Connect();

            my_header myheader = new my_header();
            myheader._lCommandID = (UInt32)EXCEL_PROTOCOL_ID.ONLY_HEAD;
            myheader._lPackSize = 0;
            myheader._lExtendData = new UInt32[] { (UInt32)EXCEL_PROTOCOL_ID.REPORT_STOCK, 0, 0 };

            byte[] buffer = StructToBytes(myheader);
            ClientPipe.Write(buffer, 0, buffer.Length);

            //ums_block_index

            byte[] headerbyte = new byte[20];
            int readCount = 0;
            my_header myheaderrecv = new my_header();
            if ((readCount = ClientPipe.Read(headerbyte, 0, 20)) > 0)
            { 
                myheaderrecv = (my_header)BytesToStruct(headerbyte, myheaderrecv.GetType());
            }
            byte[] datebyte = new byte[8613];
            while (true)
            {
                readCount = ClientPipe.Read(datebyte, 0, 8613);
                if (readCount < 0) break;
                ums_rptfunc functemp = new ums_rptfunc();
                functemp = (ums_rptfunc)BytesToStruct(datebyte, functemp.GetType());
                rptfuncs.Add(functemp);
                if (rptfuncs.Count == myheaderrecv._lExtendData[0])
                    break;
            }

            rptfuncs.TrimToSize();
            ClientPipe.Close();

        }

        public string GetCustomUI(string RibbonID)
        {
            return Properties.Resources.customUI;
        }

        Office.IRibbonUI m_Ribbon;
        public void OnLoad(Office.IRibbonUI ribbon)
        {
            m_Ribbon = ribbon;
        }

        public stdole.IPictureDisp OnGetImage(string imageName)
        {
            //Assembly assembly = Assembly.GetExecutingAssembly();

            //String projectName = Assembly.GetExecutingAssembly().GetName().Name.ToString();
            //Stream stream = assembly.GetManifestResourceStream(projectName + ".Resources." + imageName);

           // return new Bitmap(stream);

            switch (imageName)
            { 
                case "insert":
                    return ConvertImage.Convert2Picture(Resources.insert);
                case "history":
                    return ConvertImage.Convert2Picture(Resources.history);
                case "realtime":
                    return ConvertImage.Convert2Picture(Resources.realtime);
                case "report":
                    return ConvertImage.Convert2Picture(Resources.report);
                case "shot":
                    return ConvertImage.Convert2Picture(Resources.shot);
                case "refresh":
                    return ConvertImage.Convert2Picture(Resources.refresh);
                case "help":
                    return ConvertImage.Convert2Picture(Resources.help);
                case "wizzard":
                    return ConvertImage.Convert2Picture(Resources.wizzard);
                case "lr":
                    return ConvertImage.Convert2Picture(Resources.lr);
                case "file":
                    return ConvertImage.Convert2Picture(Resources.file);
                case "selected":
                    return ConvertImage.Convert2Picture(Resources.selceted);
                case "page":
                    return ConvertImage.Convert2Picture(Resources.page);
            }
            return ConvertImage.Convert2Picture(Resources.insert);
        }

        #endregion
        public void OnUMSButtonInsertFormulaClick(IRibbonControl control)
        {
            int a = 0;
        }

        public void OnUMSButtonHistoryClick(IRibbonControl control)
        {
            int a = 0;
        }

        public void OnUMSButtonRealTimeClick(IRibbonControl control)
        {
            int a = 0;
        }

        public void OnUMSButtonWizzardClick(IRibbonControl control)
        {
            OnWizzard();
        }

        public void OnUMSButtonLRClick(IRibbonControl control)
        {
            OnLR();
        }

        public void OnUMSButtonRefreshFileClick(IRibbonControl control)
        {
            int a = 0;
        }

        public void OnUMSHelpClick(IRibbonControl control)
        {
            int a = 0;
        }

        public void OnUMSButtonSnapshotPageClick(IRibbonControl control)
        {
            int a = 0;
        }

        public void OnUMSButtonSnapshotFileClick(IRibbonControl control)
        {
            int a = 0;
        }

        public void OnUMSButtonRefreshSelectedClick(IRibbonControl control)
        {
            int a = 0;
        }

        public void OnUMSButtonRefreshPageClick(IRibbonControl control)
        {
            int a = 0;
        }


        public void OnUMSButtonInsertFormulaClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }

        public void OnUMSButtonHistoryClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }

        public void OnUMSButtonRealTimeClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }

        public void OnUMSButtonWizzardClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            OnWizzard();
        }

        public void OnUMSButtonLRClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            OnLR();
        }

        public void OnUMSButtonRefreshFileClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }

        public void OnUMSHelpClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }

        public void OnUMSButtonSnapshotPageClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }

        public void OnUMSButtonSnapshotFileClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }

        public void OnUMSButtonRefreshSelectedClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }

        public void OnUMSButtonRefreshPageClick(CommandBarButton Ctrl, ref bool CancelDefault)
        {
            int a = 0;
        }
    }
}
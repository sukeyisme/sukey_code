#pragma once
// CSocketServer 对话框

class CSocketServer : public CDialog
{
	DECLARE_DYNAMIC(CSocketServer)

public:
	CSocketServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSocketServer();

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_port;
	CString m_numthread;
public:
	afx_msg void OnBnClickedRun();
};

#pragma once
// CSocketServer �Ի���

class CSocketServer : public CDialog
{
	DECLARE_DYNAMIC(CSocketServer)

public:
	CSocketServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSocketServer();

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_port;
	CString m_numthread;
public:
	afx_msg void OnBnClickedRun();
};

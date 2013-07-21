
// studyQQDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
DWORD  WINAPI RecvThread(LPVOID lpParam);
DWORD WINAPI AcceptConnect(LPVOID lpParam);
// CstudyQQDlg 对话框
class CstudyQQDlg : public CDialogEx
{
// 构造
public:
	CstudyQQDlg(CWnd* pParent = NULL);	// 标准构造函数
	SOCKET server;
	std::vector<SOCKET> clients;
	SOCKET m_curClient;
// 对话框数据
	enum { IDD = IDD_STUDYQQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 消息记录
	CListBox m_listRecord;
	afx_msg void OnBnClickedButtonStartServer();
	CString m_serverSocketString;
	afx_msg void OnBnClickedButtonConnect();
	// 连接地址
	CIPAddressCtrl m_oppAddress;
	// 连接端口
	CString m_oppPort;
	afx_msg void OnBnClickedSend();
	CEdit m_sendMessage;
	CString m_sendString;
};

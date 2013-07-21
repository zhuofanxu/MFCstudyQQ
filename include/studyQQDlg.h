
// studyQQDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
DWORD  WINAPI RecvThread(LPVOID lpParam);
DWORD WINAPI AcceptConnect(LPVOID lpParam);
// CstudyQQDlg �Ի���
class CstudyQQDlg : public CDialogEx
{
// ����
public:
	CstudyQQDlg(CWnd* pParent = NULL);	// ��׼���캯��
	SOCKET server;
	std::vector<SOCKET> clients;
	SOCKET m_curClient;
// �Ի�������
	enum { IDD = IDD_STUDYQQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ��Ϣ��¼
	CListBox m_listRecord;
	afx_msg void OnBnClickedButtonStartServer();
	CString m_serverSocketString;
	afx_msg void OnBnClickedButtonConnect();
	// ���ӵ�ַ
	CIPAddressCtrl m_oppAddress;
	// ���Ӷ˿�
	CString m_oppPort;
	afx_msg void OnBnClickedSend();
	CEdit m_sendMessage;
	CString m_sendString;
};

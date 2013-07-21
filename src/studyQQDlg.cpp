
// studyQQDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "studyQQ.h"
#include "studyQQDlg.h"
#include "afxdialogex.h"
#include "stdafx.h"
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

FD_SET mSocketSet,mSelectSet;                     //套接字集,选择集
SOCKET mSockets[64];                             //套接字数组
int    mSocketNum=0;

sockaddr addr;
int addrLen;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CstudyQQDlg 对话框




CstudyQQDlg::CstudyQQDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CstudyQQDlg::IDD, pParent)
	, m_serverSocketString(_T(""))
	, m_oppPort(_T(""))
	, m_sendString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CstudyQQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECORD, m_listRecord);
	DDX_Text(pDX, IDC_EDIT_SELF_PORT, m_serverSocketString);
	DDX_Control(pDX, IDC_IPADDRESS1, m_oppAddress);
	DDX_Text(pDX, IDC_EDIT_OPP_PORT, m_oppPort);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_sendMessage);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_sendString);
}

BEGIN_MESSAGE_MAP(CstudyQQDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START_SERVER, &CstudyQQDlg::OnBnClickedButtonStartServer)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CstudyQQDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_SEND, &CstudyQQDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CstudyQQDlg 消息处理程序

BOOL CstudyQQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->m_oppAddress.SetWindowText("127.0.0.1");
	this->m_oppPort="8080";
	this->m_serverSocketString="8080";
	UpdateData(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CstudyQQDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CstudyQQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CstudyQQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CstudyQQDlg::OnBnClickedButtonStartServer()
{
	// TODO: 在此添加控件通知处理程序代码
	char buff[256];
	WSADATA data;
	WORD version=MAKEWORD(2,2);
	::WSAStartup(version,&data);
	server=socket(AF_INET,SOCK_STREAM,0);
	
    addrLen=sizeof(sockaddr);
	UpdateData();
	CString sAddress="127.0.0.1:"+m_serverSocketString;
	::WSAStringToAddress(sAddress.GetBuffer(),AF_INET,NULL,&addr,&addrLen);
	bind(server,&addr,addrLen);
	listen(server,63);
	FD_ZERO(&mSocketSet);                  //套接字集清空
	FD_SET(server,&mSocketSet);            //将服务器套接字注入要选择的套接字集
	mSockets[0]=server;                    //套接字数组第一个元素为服务器套接字
	mSocketNum=mSocketNum+1;
	
	CreateThread(NULL,0,RecvThread,this,0,NULL);
	//closesocket(server);
	/*while(true){
		SOCKET client=accept(server,NULL,NULL);
		if(client==INVALID_SOCKET)
			break;
		while(true){
			int a=recv(client,buff,256,0);
			if(a<0)
				break;
			gets(buff);
			send(client,buff,strlen(buff),0);

		}

	}*/
	
}
DWORD WINAPI AcceptConnect(LPVOID lpParam){
	CstudyQQDlg * dlg=(CstudyQQDlg*)lpParam;
	while(true){
		SOCKET client=accept(dlg->server,NULL,NULL);
		if(client==INVALID_SOCKET)
			break;
		dlg->clients.push_back(client);
		dlg->m_curClient=client;
		CreateThread(NULL,0,RecvThread,dlg,0,0);
		/*while(true){
			int a=recv(client,buff,256,0);
			if(a<0)
				break;
			gets(buff);
			send(client,buff,strlen(buff),0);

		}*/

	}
	return 0;
}

void CstudyQQDlg::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	char buff[256];
	WORD version=MAKEWORD(2,2);
	WSADATA data;
	::WSAStartup(version,&data);
	SOCKET client=socket(AF_INET,SOCK_STREAM,0);
	sockaddr addr;
	int len=sizeof(addr);
	CString sAddress;
	UpdateData();
	this->m_oppAddress.GetWindowText(sAddress);
	sAddress=sAddress+":"+this->m_oppPort;
	::WSAStringToAddress(sAddress.GetBuffer(),AF_INET,NULL,&addr,&len);
	if(-1==connect(client,&addr,len)){
		MessageBox("连接不成功，请检查地址正确性\n"+sAddress);
	}


	//for(int i=1;i<=mSocketNum;i++)
	//	{
	 
	   //}
	clients.push_back(client);                 //将新客户端套接字保存到数组
	m_curClient=client;
	CreateThread(NULL,0,RecvThread,this,0,NULL);
	
}
DWORD WINAPI RecvThread(LPVOID lpParam){
	//char recvBuff[256];
	
	CstudyQQDlg * dlg=(CstudyQQDlg*)lpParam;
	SOCKET s=dlg->m_curClient;
	
	select(FD_SETSIZE,&mSelectSet,NULL,NULL,NULL);
		while(true)
		{
			mSelectSet=mSocketSet;

			if(FD_ISSET(mSockets[0],&mSelectSet))
			{
				mSockets[mSocketNum]=accept(mSockets[0],&addr,&addrLen);
				mSocketNum++;
			}
		    for(int i=1;i<=mSocketNum;i++)
		        {
			       
			        
				         char buff[1024];//用于保存接收到的消息
				         int messageLen=::recv(mSockets[i],buff,1024,0);//从客户端接收消息
				         if(messageLen<0)continue;
			             buff[messageLen]=0;
					//printf("recv:%s\n",buff);//显示收到的消息
					     dlg->m_listRecord.AddString(buff);
					    // send(mSockets[i],buff,messageLen,0);
				           
						 


	                
			    }
	           
	   	     
	/*while(true){
		for(int i=0;i<dlg->clients.size();i++){
			int len=recv(s,recvBuff,255,0);
			if(len<0) continue;
			recvBuff[len]=0;
			dlg->m_listRecord.AddString(recvBuff);
		}
	}*/
}
}

void CstudyQQDlg::OnBnClickedSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	for(int i=0;i<clients.size();i++)
		send(clients[i],m_sendString.GetBuffer(),m_sendString.GetLength(),0);
	
}


// studyQQDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

FD_SET mSocketSet,mSelectSet;                     //�׽��ּ�,ѡ��
SOCKET mSockets[64];                             //�׽�������
int    mSocketNum=0;

sockaddr addr;
int addrLen;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CstudyQQDlg �Ի���




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


// CstudyQQDlg ��Ϣ�������

BOOL CstudyQQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	this->m_oppAddress.SetWindowText("127.0.0.1");
	this->m_oppPort="8080";
	this->m_serverSocketString="8080";
	UpdateData(false);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CstudyQQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CstudyQQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CstudyQQDlg::OnBnClickedButtonStartServer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	FD_ZERO(&mSocketSet);                  //�׽��ּ����
	FD_SET(server,&mSocketSet);            //���������׽���ע��Ҫѡ����׽��ּ�
	mSockets[0]=server;                    //�׽��������һ��Ԫ��Ϊ�������׽���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("���Ӳ��ɹ��������ַ��ȷ��\n"+sAddress);
	}


	//for(int i=1;i<=mSocketNum;i++)
	//	{
	 
	   //}
	clients.push_back(client);                 //���¿ͻ����׽��ֱ��浽����
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
			       
			        
				         char buff[1024];//���ڱ�����յ�����Ϣ
				         int messageLen=::recv(mSockets[i],buff,1024,0);//�ӿͻ��˽�����Ϣ
				         if(messageLen<0)continue;
			             buff[messageLen]=0;
					//printf("recv:%s\n",buff);//��ʾ�յ�����Ϣ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	for(int i=0;i<clients.size();i++)
		send(clients[i],m_sendString.GetBuffer(),m_sendString.GetLength(),0);
	
}

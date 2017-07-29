// KTMNGJDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KTMNGJ.h"
#include "KTMNGJDlg.h"
#include ".\ktmngjdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE hMuTex;

//发送数据帧的从机应答
void CKTMNGJDlg::SendDataAnswer()
{
	int nTmp;
 	BYTE bTmp[21];
 	ZeroMemory(bTmp, 21);
 	bTmp[0] = *((BYTE *)&m_nAddr);
	bTmp[0]++;
 	bTmp[0] |= 0x08;         //第八位置为1
 	bTmp[1] = 0x83;
 	bTmp[2] = 0x11;
	nTmp = m_AU * 10;
 	BYTE* pTmp = (BYTE*)&nTmp;
 	bTmp[3] = *pTmp;
 	pTmp++;
 	bTmp[4] = *pTmp;
	
	nTmp = m_BU * 10;
 	pTmp = (BYTE*)&nTmp;
 	bTmp[5] = *pTmp;
 	pTmp++;
 	bTmp[6] = *pTmp;
 
	nTmp = m_CU * 10;
 	pTmp = (BYTE*)&nTmp;
 	bTmp[7] = *pTmp;
 	pTmp++;
 	bTmp[8] = *pTmp;
 
	nTmp = m_AI * 10;
 	pTmp = (BYTE*)&nTmp;
 	bTmp[9] = *pTmp;
 	pTmp++;
 	bTmp[10] = *pTmp;
 
	nTmp = m_BI * 10;
 	pTmp = (BYTE*)&nTmp;
 	bTmp[11] = *pTmp;
 	pTmp++;
 	bTmp[12] = *pTmp;
 
	nTmp = m_CI * 10;
 	pTmp = (BYTE*)&nTmp;
 	bTmp[13] = *pTmp;
 	pTmp++;
 	bTmp[14] = *pTmp;
 
	nTmp = m_WD * 10;
 	pTmp = (BYTE*)&nTmp;
 	bTmp[15] = *pTmp;
 	pTmp++;
 	bTmp[16] = *pTmp;
 
	nTmp = (BYTE)m_SW * 100;
 	pTmp = (BYTE*)&nTmp;
 	bTmp[17] = *pTmp;
 	pTmp++;
 	bTmp[18] = *pTmp;
 	bTmp[19] = 0x00;
 	BYTE bTmp2;
 	SetYJM(bTmp2, bTmp, 20);
 	bTmp[20] = bTmp2;
	DWORD nLen = 0;
 	if(!m_KTcom.WriteComm(bTmp, 21, &nLen))
 	{
 		TRACE("发送失败");
 	}
}
//发送控制命令的从机应答
void CKTMNGJDlg::SendCtrlAnswer()
{
	BYTE bTmp[5];
	ZeroMemory(bTmp, 5);
	bTmp[0] = *((BYTE *)&m_nAddr);
	bTmp[0]++;
	bTmp[0] |= 0x80;         //第八位置为1
	bTmp[1] = 0x84;
	bTmp[2] = 0x01;
	bTmp[3] = 0x00;
	BYTE bTmp2;
    SetYJM(bTmp2, bTmp, 4);
	bTmp[4] = bTmp2;
	DWORD nLen = 0;
	if (!m_KTcom.WriteComm(bTmp, 5, &nLen))
	{
		TRACE("发送失败");
	}
	
}
//填充串口参数
BOOL CKTMNGJDlg::IniComInfo()    
{
	if (m_KTcom.SetPara( m_KTCommPara.CommNo,
		                 m_KTCommPara.Rate,
		                 m_KTCommPara.Jylx,
		                 m_KTCommPara.Sjw,
		                 m_KTCommPara.Tzw))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//打开串口
BOOL CKTMNGJDlg::OpenCom()
{
	if (m_KTcom.Open())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//关闭端口
BOOL CKTMNGJDlg::CloseCom()
{
	if (m_KTcom.Close())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//初始化 串口 结构体
void CKTMNGJDlg::InitInfo()
{
	UpdateData(TRUE);
// 	GetDlgItem(IDC_COMBO_PORT)->GetWindowText(m_KTCommPara.CommNo);  //端口号
// 	GetDlgItem(IDC_COMBO_ADDR)->GetWindowText(m_KTCommPara.bAddress);//地址
	m_KTCommPara.bAddress = (BYTE)m_nAddr + 1;
	m_KTCommPara.CommNo = (BYTE)m_nPort + 1;
	m_KTCommPara.Rate = 9600;
	m_KTCommPara.Jylx = 3;
	m_KTCommPara.Sjw = 8;
	m_KTCommPara.Tzw = 0;
	m_KTCommPara.WRate = 300;
	m_bFrameLen = 0;
	
}
//生成校验码
void CKTMNGJDlg::SetYJM(BYTE& bYJM, BYTE* pBuf, int nLen)
{
	BYTE bTmp = 0x00;
	for (int i = 0 ; i < nLen ; i++)
	{
		bTmp ^= pBuf[i];
	}
	bYJM = bTmp;
}
//异或校验
BOOL CKTMNGJDlg::YHJY(BYTE* pBuf, BYTE bLen, BYTE bJy)
{
	BYTE* pByte = new BYTE;
	ZeroMemory(pByte, 1);
	for (BYTE i = 0 ; i < bLen ; i++)
	{
		*pByte ^= pBuf[i];
	}
	*pByte ^= bJy;
	if ( 0 == *pByte )
	{
		delete pByte;
		return TRUE;
	}
	else
	{
		delete pByte;
		return FALSE;
	}
}

//查找帧开始字节
int CKTMNGJDlg::FindByte(BYTE* pBuf, BYTE* pFind, int nByteLen)
{
	int location = -1 ;
	BYTE* pTmp = new BYTE;
	for ( int i = 0 ; i < nByteLen ; i++ )
	{
		*pTmp = pBuf[i];
		*pTmp &= 0x7f;
		*pTmp ^= *pFind;      //低七位相等
		if (0 == *pTmp)
		{
			location = i;
			break;
		}
		ZeroMemory(pTmp, 1);
	}
	delete pTmp;
	pTmp = NULL;
	return location ;

}
//处理空调控制命令
void CKTMNGJDlg::DealCtrlCmd(BYTE* pBuf, BYTE bFrameLen)
{
	BYTE bData = pBuf[3];
	bData &= 0xf0;                //取高四位
	switch (bData)
	{
	case 0x00:
		{
			GetDlgItem(IDC_STATIC_ZL)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_ZLWD)->EnableWindow(TRUE);
			bData = pBuf[3];
			bData &= 0x0f;
			bData += 16;
			char strTmp[5];
			itoa(bData, strTmp, 10);
			GetDlgItem(IDC_STATIC_ZLWD)->SetWindowText(strTmp);
			GetDlgItem(IDC_STATIC_YKK)->EnableWindow(TRUE);
			break;
		}
	case 0x10:
		{
			GetDlgItem(IDC_STATIC_ZR)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_ZRWD)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_YKK)->EnableWindow(TRUE);
			bData = pBuf[3];
			bData &= 0x0f;        //取低四位
			bData += 16;
			char strTmp[5];
			itoa(bData, strTmp, 10);
			GetDlgItem(IDC_STATIC_ZRWD)->SetWindowText(strTmp);
			break;
		}
	case 0x20:
		{
			GetDlgItem(IDC_STATIC_YKK)->EnableWindow(TRUE);
			bData = pBuf[3];
			bData &= 0x0f;
			switch (bData)
			{
			case 0x00:
				{
					GetDlgItem(IDC_STATIC_CS)->EnableWindow(TRUE);
					break;
				}
			case 0x01:
				{
					GetDlgItem(IDC_STATIC_ZD)->EnableWindow(TRUE);
					break;
				}
			case 0x02:
				{
					break;
				}
			case 0x03:
				{
					GetDlgItem(IDC_STATIC_YKG)->EnableWindow(TRUE);
					GetDlgItem(IDC_STATIC_YKK)->EnableWindow(FALSE);
					break;
				}
			default:
				break;
			}
			break;
		}
	default:
		break;

	}

}
//接收发送线程
UINT CKTMNGJDlg::RecSendThd( LPVOID pParam )
{
	CKTMNGJDlg* pMainWnd = (CKTMNGJDlg* )pParam;
	DWORD nRead = 0;
	BOOL bRet;
	while (TRUE)
	{
		pMainWnd->m_pRecBuf = new BYTE[BUFLEN];
		if (NULL == pMainWnd->m_pRecBuf)
		{
			AfxMessageBox("申请内存失败");
		}
		ZeroMemory(pMainWnd->m_pRecBuf, BUFLEN);
		BYTE* pCur = pMainWnd->m_pRecBuf;            //工作指针
		if (!pMainWnd->m_ThdFlag)
		{
			bRet = pMainWnd->m_KTcom.ReceData(nRead, pMainWnd->m_pRecBuf);    //接收数据
		}
		if (nRead > 0)
		{
			BYTE* pFrameBegin = new BYTE ;
			ZeroMemory(pFrameBegin, 1) ;
			*pFrameBegin = pMainWnd->m_KTCommPara.bAddress ;
			*pFrameBegin &= 0x7f ;                                  //屏蔽第八位
			int location = -1;
			int nSum = 0;
			{
				location = pMainWnd->FindByte(pCur, pFrameBegin, nRead);  //找的的是低七位相等
				
				if ( location >= 0 )
				{
					pCur += location ;                    //pCur 指向帧的开始
					//判断第八位
					BYTE bTmp = pCur[0] & 0x80;            //屏蔽低七位
					switch (bTmp)
					{
					case 0x00:
						  {
							  if (0x03 == pCur[1])
							  {
								  pMainWnd->m_bFrameLen = 3;   //采集数据帧

							  }
							  else
							  {
								  pMainWnd->m_bFrameLen = 0;
								  TRACE("帧错误");
							  }
							  break;
						  }
					case 0x80:
						  {
								if (0x04 == pCur[1])
								{
									pMainWnd->m_bFrameLen = 5;   //空调控制命令帧

								}
								else
								{
									pMainWnd->m_bFrameLen = 0;
									TRACE("帧错误");
								}
								break;
						  }
					default:
						break;
					}
					BYTE* pYj = new BYTE ;
					ZeroMemory(pYj, 1);
					*pYj = pCur[pMainWnd->m_bFrameLen - 1];  //最后一字节是校验码
					if ( !pMainWnd->YHJY(pCur, pMainWnd->m_bFrameLen - 1, *pYj) )
					{
						TRACE("校验错误");
						delete pYj ;
						pYj = NULL ;
					}
					else
					{
						switch (pMainWnd->m_bFrameLen)
						{
						case 3:
							{
								::WaitForSingleObject(hMuTex, INFINITE);
								pMainWnd->SendDataAnswer();
								::ReleaseMutex(hMuTex);
								break;
							}
						case 5:
							{
								//初始化界面
								pMainWnd->GetDlgItem(IDC_STATIC_ZL)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_ZR)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_CS)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_ZD)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_YKG)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_ZLWD)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_ZRWD)->EnableWindow(FALSE);
								pMainWnd->DealCtrlCmd(pCur, 5);        //显示界面
								//从机应答
								pMainWnd->SendCtrlAnswer();
								break;
							}
						default:
							break;
						}
					}
					delete pYj;
					pYj = NULL;
				}
			}
			delete pFrameBegin;
			pFrameBegin = NULL;
		}
        Sleep(100);		
		delete [] pMainWnd->m_pRecBuf;
		pMainWnd->m_pRecBuf = NULL;
	}
	return 0;
}

//
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CKTMNGJDlg 对话框



CKTMNGJDlg::CKTMNGJDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKTMNGJDlg::IDD, pParent)
	, m_nAddr(0)
	, m_nPort(0)
	, m_AU(200)
	, m_BU(200)
	, m_CU(200)
	, m_AI(10)
	, m_BI(10)
	, m_CI(10)
	, m_WD(26)
	, m_SW(1.6)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//AfxGetApp()->m_pMainWnd->SetWindowText( "空调控制模拟工具" ) ;
}

void CKTMNGJDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_ADDR, m_nAddr);
	DDX_CBIndex(pDX, IDC_COMBO_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT1, m_AU);
	DDX_Text(pDX, IDC_EDIT2, m_BU);
	DDX_Text(pDX, IDC_EDIT3, m_CU);
	DDX_Text(pDX, IDC_EDIT4, m_AI);
	DDX_Text(pDX, IDC_EDIT5, m_BI);
	DDX_Text(pDX, IDC_EDIT6, m_CI);
	DDX_Text(pDX, IDC_EDIT7, m_WD);
	DDX_Text(pDX, IDC_EDIT8, m_SW);
	DDX_Control(pDX, IDC_COMBO_ADDR, m_ComboAddr);
	DDX_Control(pDX, IDC_COMBO_PORT, m_ComboPort);
}

BEGIN_MESSAGE_MAP(CKTMNGJDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_COLLECT, OnBnClickedButtonCollect)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnBnClickedButtonModify)
END_MESSAGE_MAP()


// CKTMNGJDlg 消息处理程序

BOOL CKTMNGJDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
// 	int a[2];
// 	a[0] = 234;
	AfxGetApp()->m_pMainWnd->SetWindowText( "空调控制模拟工具" ) ;
	GetDlgItem(IDC_STATIC_ZL)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ZR)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CS)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ZD)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_YKG)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_YKK)->EnableWindow(FALSE);
//	GetDlgItem(IDC_EDIT_ZL)->EnableWindow(FALSE);
//	GetDlgItem(IDC_EDIT_ZR)->EnableWindow(FALSE);
	m_ComboAddr.SetCurSel(9);
	m_ComboPort.SetCurSel(2);

// 	int  a = 90;
//     char b[10] ;
// 	itoa(a, b, 10);
// 
// 	GetDlgItem(IDC_EDIT_ZL)->SetWindowText(b);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CKTMNGJDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKTMNGJDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CKTMNGJDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKTMNGJDlg::OnBnClickedButtonCollect()
{
	// TODO: 在此添加控件通知处理程序代码
	InitInfo();

	CString strTmp;
	GetDlgItem(IDC_BUTTON_COLLECT)->GetWindowText(strTmp);
	if (strTmp == "打开")
	{
		GetDlgItem(IDC_COMBO_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ADDR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_COLLECT)->SetWindowText("关闭");
		m_ThdFlag = FALSE;  //说明县城已经启动
		if (!IniComInfo())
		{
			AfxMessageBox("初始化串口失败");
			m_BTXFlag = FALSE;
			return;
		}
		else
		{
			m_BTXFlag = TRUE;
		}

		if (!OpenCom())
		{
			AfxMessageBox("打开串口失败");
			m_BTXFlag = FALSE;
			return;
		}
		else
		{
			m_BTXFlag = TRUE;
		}
		CWinThread* pThd = AfxBeginThread(RecSendThd,
			this,
			THREAD_PRIORITY_NORMAL,
			0,
			CREATE_SUSPENDED);
		if ( NULL == pThd )
		{
			AfxMessageBox("启动线程失败");
			return;
		}
		m_pRecSendThd = pThd;
		pThd->ResumeThread();
	}
	if (strTmp == "关闭")
	{
		GetDlgItem(IDC_COMBO_ADDR)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_YKK)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_YKG)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ZL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ZR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_CS)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ZD)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ZLWD)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ZRWD)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_COLLECT)->SetWindowText("打开");
		m_ThdFlag = TRUE;
		if (!m_KTcom.Close())
		{
			TRACE("关闭串口失败");
			return;
		}
	}

}

void CKTMNGJDlg::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	::WaitForSingleObject(hMuTex, INFINITE);
	UpdateData(TRUE);
	::ReleaseMutex(hMuTex);

}

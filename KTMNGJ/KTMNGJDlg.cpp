// KTMNGJDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KTMNGJ.h"
#include "KTMNGJDlg.h"
#include ".\ktmngjdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE hMuTex;

//��������֡�Ĵӻ�Ӧ��
void CKTMNGJDlg::SendDataAnswer()
{
	int nTmp;
 	BYTE bTmp[21];
 	ZeroMemory(bTmp, 21);
 	bTmp[0] = *((BYTE *)&m_nAddr);
	bTmp[0]++;
 	bTmp[0] |= 0x08;         //�ڰ�λ��Ϊ1
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
 		TRACE("����ʧ��");
 	}
}
//���Ϳ�������Ĵӻ�Ӧ��
void CKTMNGJDlg::SendCtrlAnswer()
{
	BYTE bTmp[5];
	ZeroMemory(bTmp, 5);
	bTmp[0] = *((BYTE *)&m_nAddr);
	bTmp[0]++;
	bTmp[0] |= 0x80;         //�ڰ�λ��Ϊ1
	bTmp[1] = 0x84;
	bTmp[2] = 0x01;
	bTmp[3] = 0x00;
	BYTE bTmp2;
    SetYJM(bTmp2, bTmp, 4);
	bTmp[4] = bTmp2;
	DWORD nLen = 0;
	if (!m_KTcom.WriteComm(bTmp, 5, &nLen))
	{
		TRACE("����ʧ��");
	}
	
}
//��䴮�ڲ���
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
//�򿪴���
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
//�رն˿�
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
//��ʼ�� ���� �ṹ��
void CKTMNGJDlg::InitInfo()
{
	UpdateData(TRUE);
// 	GetDlgItem(IDC_COMBO_PORT)->GetWindowText(m_KTCommPara.CommNo);  //�˿ں�
// 	GetDlgItem(IDC_COMBO_ADDR)->GetWindowText(m_KTCommPara.bAddress);//��ַ
	m_KTCommPara.bAddress = (BYTE)m_nAddr + 1;
	m_KTCommPara.CommNo = (BYTE)m_nPort + 1;
	m_KTCommPara.Rate = 9600;
	m_KTCommPara.Jylx = 3;
	m_KTCommPara.Sjw = 8;
	m_KTCommPara.Tzw = 0;
	m_KTCommPara.WRate = 300;
	m_bFrameLen = 0;
	
}
//����У����
void CKTMNGJDlg::SetYJM(BYTE& bYJM, BYTE* pBuf, int nLen)
{
	BYTE bTmp = 0x00;
	for (int i = 0 ; i < nLen ; i++)
	{
		bTmp ^= pBuf[i];
	}
	bYJM = bTmp;
}
//���У��
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

//����֡��ʼ�ֽ�
int CKTMNGJDlg::FindByte(BYTE* pBuf, BYTE* pFind, int nByteLen)
{
	int location = -1 ;
	BYTE* pTmp = new BYTE;
	for ( int i = 0 ; i < nByteLen ; i++ )
	{
		*pTmp = pBuf[i];
		*pTmp &= 0x7f;
		*pTmp ^= *pFind;      //����λ���
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
//����յ���������
void CKTMNGJDlg::DealCtrlCmd(BYTE* pBuf, BYTE bFrameLen)
{
	BYTE bData = pBuf[3];
	bData &= 0xf0;                //ȡ����λ
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
			bData &= 0x0f;        //ȡ����λ
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
//���շ����߳�
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
			AfxMessageBox("�����ڴ�ʧ��");
		}
		ZeroMemory(pMainWnd->m_pRecBuf, BUFLEN);
		BYTE* pCur = pMainWnd->m_pRecBuf;            //����ָ��
		if (!pMainWnd->m_ThdFlag)
		{
			bRet = pMainWnd->m_KTcom.ReceData(nRead, pMainWnd->m_pRecBuf);    //��������
		}
		if (nRead > 0)
		{
			BYTE* pFrameBegin = new BYTE ;
			ZeroMemory(pFrameBegin, 1) ;
			*pFrameBegin = pMainWnd->m_KTCommPara.bAddress ;
			*pFrameBegin &= 0x7f ;                                  //���εڰ�λ
			int location = -1;
			int nSum = 0;
			{
				location = pMainWnd->FindByte(pCur, pFrameBegin, nRead);  //�ҵĵ��ǵ���λ���
				
				if ( location >= 0 )
				{
					pCur += location ;                    //pCur ָ��֡�Ŀ�ʼ
					//�жϵڰ�λ
					BYTE bTmp = pCur[0] & 0x80;            //���ε���λ
					switch (bTmp)
					{
					case 0x00:
						  {
							  if (0x03 == pCur[1])
							  {
								  pMainWnd->m_bFrameLen = 3;   //�ɼ�����֡

							  }
							  else
							  {
								  pMainWnd->m_bFrameLen = 0;
								  TRACE("֡����");
							  }
							  break;
						  }
					case 0x80:
						  {
								if (0x04 == pCur[1])
								{
									pMainWnd->m_bFrameLen = 5;   //�յ���������֡

								}
								else
								{
									pMainWnd->m_bFrameLen = 0;
									TRACE("֡����");
								}
								break;
						  }
					default:
						break;
					}
					BYTE* pYj = new BYTE ;
					ZeroMemory(pYj, 1);
					*pYj = pCur[pMainWnd->m_bFrameLen - 1];  //���һ�ֽ���У����
					if ( !pMainWnd->YHJY(pCur, pMainWnd->m_bFrameLen - 1, *pYj) )
					{
						TRACE("У�����");
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
								//��ʼ������
								pMainWnd->GetDlgItem(IDC_STATIC_ZL)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_ZR)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_CS)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_ZD)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_YKG)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_ZLWD)->EnableWindow(FALSE);
								pMainWnd->GetDlgItem(IDC_STATIC_ZRWD)->EnableWindow(FALSE);
								pMainWnd->DealCtrlCmd(pCur, 5);        //��ʾ����
								//�ӻ�Ӧ��
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CKTMNGJDlg �Ի���



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
	//AfxGetApp()->m_pMainWnd->SetWindowText( "�յ�����ģ�⹤��" ) ;
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


// CKTMNGJDlg ��Ϣ�������

BOOL CKTMNGJDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
// 	int a[2];
// 	a[0] = 234;
	AfxGetApp()->m_pMainWnd->SetWindowText( "�յ�����ģ�⹤��" ) ;
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
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CKTMNGJDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CKTMNGJDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKTMNGJDlg::OnBnClickedButtonCollect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	InitInfo();

	CString strTmp;
	GetDlgItem(IDC_BUTTON_COLLECT)->GetWindowText(strTmp);
	if (strTmp == "��")
	{
		GetDlgItem(IDC_COMBO_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ADDR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_COLLECT)->SetWindowText("�ر�");
		m_ThdFlag = FALSE;  //˵���س��Ѿ�����
		if (!IniComInfo())
		{
			AfxMessageBox("��ʼ������ʧ��");
			m_BTXFlag = FALSE;
			return;
		}
		else
		{
			m_BTXFlag = TRUE;
		}

		if (!OpenCom())
		{
			AfxMessageBox("�򿪴���ʧ��");
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
			AfxMessageBox("�����߳�ʧ��");
			return;
		}
		m_pRecSendThd = pThd;
		pThd->ResumeThread();
	}
	if (strTmp == "�ر�")
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

		GetDlgItem(IDC_BUTTON_COLLECT)->SetWindowText("��");
		m_ThdFlag = TRUE;
		if (!m_KTcom.Close())
		{
			TRACE("�رմ���ʧ��");
			return;
		}
	}

}

void CKTMNGJDlg::OnBnClickedButtonModify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::WaitForSingleObject(hMuTex, INFINITE);
	UpdateData(TRUE);
	::ReleaseMutex(hMuTex);

}

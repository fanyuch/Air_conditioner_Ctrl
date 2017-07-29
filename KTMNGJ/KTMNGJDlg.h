// KTMNGJDlg.h : ͷ�ļ�
//

#pragma once
#include "Comm232.h"
#include "afxwin.h"

#define        CJSH_FRAME                  1           //�ɼ�����֡����
#define        HDCJSH_FRAME                2           //�ش�ɼ�����֡����
#define        YKML_FRAME                  3           //ң������֡����
#define        YKMLFH_FRAME                4           //ң�������֡����
#define        BUFLEN                      1024        //����������


typedef struct
{
	BYTE CommNo;                 //�˿ں�
	WORD Rate;                   //������
	BYTE Jylx;                   //�˿�У�鷽ʽ
	BYTE Sjw;                    //����λ   
	BYTE Tzw;                    //ֹͣλ
	BYTE Wjy;                    //�ⲿУ�鷽ʽ
	WORD WRate;                  //�ⲿ�ٶ�
	BYTE Head[10];               //����ͷ
	BYTE HeadLen;
	BYTE Tail[10];               //����β
	BYTE TailLen; 
	BYTE bAddress;               //��ַ
} CommInfoTemplate;              //���ڽṹ
// CKTMNGJDlg �Ի���
class CKTMNGJDlg : public CDialog
{
public:
	CommInfoTemplate                      m_KTCommPara;          //��¼ ������Ϣ�Ľṹ��
    BYTE*                                 m_pRecBuf;             //���ջ�����
	CWinThread*                           m_pRecSendThd;         //���ͽ����߳�
	Comm232                               m_KTcom;               //232���ڶ���
	BOOL                                  m_BTXFlag;             //ͨ�ű�ʶ
	BYTE                                  m_bFrameLen;           //�յ���֡�ĳ���
    BOOL                                  m_ThdFlag;             //�̱߳�ʶ

// ����
public:
	CKTMNGJDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_KTMNGJ_DIALOG };

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
	void InitInfo(void);                                    //��ʼ����̬����
	BOOL IniComInfo(void);                                  //��ʼ������
	BOOL OpenCom(void);                                     //�򿪴���
	BOOL CloseCom(void);                                    //�رմ���
	static UINT RecSendThd(LPVOID pParam);                  //�����߳�
	afx_msg void OnBnClickedButtonCollect();
	BOOL YHJY(BYTE* pBuf, BYTE bLen, BYTE bJy);             //���У��
	int FindByte(BYTE* pBuf, BYTE* pFind, int nByteLen);    //����֡��ʼ�ֽ�
	void DealCtrlCmd(BYTE* pBuf, BYTE bFrameLen);           //�����������
	void SetYJM(BYTE & bYJM, BYTE* pBuf, int nLen);         //����У����
	void SendCtrlAnswer(void);                              //Ӧ�����֡
	void SendDataAnswer(void);                              //Ӧ������֡
	
	int m_nAddr;
	int m_nPort;
	int m_AU;
	int m_BU;
	int m_CU;
	int m_AI;
	int m_BI;
	int m_CI;
	int m_WD;
	float m_SW;
	CComboBox m_ComboAddr;
	CComboBox m_ComboPort;
	afx_msg void OnBnClickedButtonModify();
};

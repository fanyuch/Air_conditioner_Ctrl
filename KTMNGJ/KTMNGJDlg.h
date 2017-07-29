// KTMNGJDlg.h : 头文件
//

#pragma once
#include "Comm232.h"
#include "afxwin.h"

#define        CJSH_FRAME                  1           //采集数据帧类型
#define        HDCJSH_FRAME                2           //回答采集数据帧类型
#define        YKML_FRAME                  3           //遥控命令帧类型
#define        YKMLFH_FRAME                4           //遥控命令返回帧类型
#define        BUFLEN                      1024        //缓冲区长度


typedef struct
{
	BYTE CommNo;                 //端口号
	WORD Rate;                   //波特率
	BYTE Jylx;                   //端口校验方式
	BYTE Sjw;                    //数据位   
	BYTE Tzw;                    //停止位
	BYTE Wjy;                    //外部校验方式
	WORD WRate;                  //外部速度
	BYTE Head[10];               //数据头
	BYTE HeadLen;
	BYTE Tail[10];               //数据尾
	BYTE TailLen; 
	BYTE bAddress;               //地址
} CommInfoTemplate;              //串口结构
// CKTMNGJDlg 对话框
class CKTMNGJDlg : public CDialog
{
public:
	CommInfoTemplate                      m_KTCommPara;          //记录 串口信息的结构体
    BYTE*                                 m_pRecBuf;             //接收缓冲区
	CWinThread*                           m_pRecSendThd;         //发送接收线程
	Comm232                               m_KTcom;               //232串口对象
	BOOL                                  m_BTXFlag;             //通信标识
	BYTE                                  m_bFrameLen;           //收到的帧的长度
    BOOL                                  m_ThdFlag;             //线程标识

// 构造
public:
	CKTMNGJDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_KTMNGJ_DIALOG };

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
	void InitInfo(void);                                    //初始化静态数据
	BOOL IniComInfo(void);                                  //初始化串口
	BOOL OpenCom(void);                                     //打开串口
	BOOL CloseCom(void);                                    //关闭串口
	static UINT RecSendThd(LPVOID pParam);                  //接收线程
	afx_msg void OnBnClickedButtonCollect();
	BOOL YHJY(BYTE* pBuf, BYTE bLen, BYTE bJy);             //异或校验
	int FindByte(BYTE* pBuf, BYTE* pFind, int nByteLen);    //查找帧开始字节
	void DealCtrlCmd(BYTE* pBuf, BYTE bFrameLen);           //处理控制命令
	void SetYJM(BYTE & bYJM, BYTE* pBuf, int nLen);         //生成校验码
	void SendCtrlAnswer(void);                              //应答控制帧
	void SendDataAnswer(void);                              //应答数据帧
	
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

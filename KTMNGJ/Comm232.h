///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////

#if !defined(AFX_COMM232_H__EB5D4C44_CB43_11D4_B6C2_0080C8F18B7A__INCLUDED_)
#define AFX_COMM232_H__EB5D4C44_CB43_11D4_B6C2_0080C8F18B7A__INCLUDED_

#define   MaxBufLen    10000
#define   MaxHeadLen   10

typedef unsigned char BYTE;

enum TJylx 
{
	jno,
	jcrc,
	jbte,
	jdbte,
};

class Comm232
{
public:               //private:
	int     ReceBgnLen, ReceEndLen;
	int	    SendBgnLen, SendEndLen;
	BYTE    ReceBgn[MaxHeadLen];
	BYTE    ReceEnd[MaxHeadLen];
	BYTE    SendBgn[MaxHeadLen];
	BYTE    SendEnd[MaxHeadLen];
	BYTE    ComBuf[MaxBufLen];

	int     BgnPos, EndPos;
	int     BufLen;

	int		PortNum;
	int		BaudRate;
	int     parity;
	int     bytesize;
    int     stopbit;
	int     rts;
	int     dtr;

	HANDLE  hCom;
	BOOL    PortOpen;
	TJylx   Jylx;

	int  FindBytes(BYTE* pBuf,int FindLen,int Bgn);
	int  FindBytes(BYTE* pBuf2Fnd,int FindLen, BYTE *pBuf, int nBuf, int Bgn);
	BOOL InitComm();
	BOOL WriteComm(void* lpBuffer,DWORD nToWrite,DWORD* nWritten);
	BOOL ReadComm(void* lpBuffer,DWORD nToRead,DWORD* nRead);
	BOOL CloseComm();

public:	
	Comm232();
	~Comm232();
	BOOL SetJyLx(TJylx Lx);
	TJylx GetJylx();
	BOOL SetRecedBgn(BYTE* pReceBgn,int Len);
	BOOL SetReceEnd(BYTE* pReceEnd,int Len);
	BOOL SetSendBgn(BYTE* pSendBgn,int Len);
	BOOL SetSendEnd(BYTE* pSendEnd,int Len);
	void GetRecedBgn(BYTE* pReceBgn,int *Len);
	void GetReceEnd(BYTE* pReceEnd,int *Len);
	void GetSendBgn(BYTE* pSendBgn,int *Len);
	void GetSendEnd(BYTE* pSendEnd,int *Len);

	BOOL PortIsOpen();
	BOOL SetPortNum(int ComNum);
    BOOL SetPara(int ComNum,int Baud,int par,int bytes,int stop);
	void GetPara(int* ComNum,int *Baud);
	BOOL Open();
	BOOL Close();
	BOOL ReceData(BYTE* ReceBuf,int Len,int *ReceLen);
	/// 为适应自己的需要，重载ReceData (BYTE *ReceBuf)
	BOOL ReceData (DWORD &nRead, BYTE *pRecBuf, int nLen = 1024);
	BOOL SendData(BYTE* SendBuf,int Len);

	BOOL SetRts(int iRts);
	BOOL SetDtr(int iDtr);

	BOOL SetStopBit(int iStop);
public:
	void PurgeComRcvBuf();
	void PurgeComSndBuf();
	BOOL CRCCheck(BYTE* pBuf,int Len);

};

#endif // !defined(AFX_COMM232_H__EB5D4C44_CB43_11D4_B6C2_0080C8F18B7A__INCLUDED_)
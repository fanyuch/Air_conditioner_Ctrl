#include "stdafx.h"
#include "Comm232.h"
//CRCѭ������У��������
WORD CRC(BYTE* pBuf,int Len)
{
    WORD CRC = 0;
    BYTE R;
    int i,j,k,m;

    CRC=0;
	
	if(Len<=0) return 0; 

    for(i=0;i<Len;i++)
    {
		R=pBuf[i];
		for(j=0;j<8;j++)
		{
			if (R>127) k=1;
			else k=0;

			R<<=1;

			if(CRC>0x7fff) m=1;
			else m=0;

			if (k+m==1) k=1;
			else k=0;
			
			CRC<<=1;

			if (k==1) CRC^=0x1021;
		}
    }
    return CRC; 
}

//���ֺ�У��
BOOL SZHCRC(BYTE* pBuf,int Len)
{
    BYTE CRC=0;
	Len=Len-1;
	for(int i=0;i<Len;i++)
	{
       CRC=CRC+pBuf[i];
	}
	if(CRC==pBuf[Len]) return TRUE;
	return FALSE; 
}

//˫�ֺ�У��
BOOL DZHCRC(BYTE* pBuf,int Len)
{
    Len=Len-2;
	WORD CRC=0;
	for(int i=0;i<Len;i=i++)
	{
		CRC=CRC+pBuf[i]; 
	}
	if(CRC==(pBuf[Len]*256+pBuf[Len+1])) return TRUE;
	return FALSE ;
}
//У�鷽ʽ
BOOL Comm232::CRCCheck(BYTE* pBuf,int Len)
{
	if(Len<2) 
		return FALSE;
	//��У�� 
    if(Jylx==jno) 
		return TRUE;  
    //1��ʾCRC
	if(Jylx==jcrc)  
		return (CRC(pBuf,Len)==0);  
    //2-���ֽں�У��
	if(Jylx==jbte)  
		return SZHCRC(pBuf,Len);
    //3��ʾ˫�ֽں�У��
	if(Jylx==jdbte) 
		return DZHCRC(pBuf,Len);
	return TRUE;
}

BOOL Comm232::InitComm()
{
    char ComStr[80];
	DCB  dcb;
	COMMTIMEOUTS TimeOut;
	sprintf(ComStr,"\\\\.\\Com%d",PortNum);

    hCom=CreateFile(ComStr,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
    if (hCom==INVALID_HANDLE_VALUE) return FALSE;
    SetupComm(hCom,1500,150);
    
	if (!GetCommState(hCom,&dcb)) return FALSE;
    dcb.BaudRate=BaudRate;
	dcb.ByteSize=bytesize;  //λ��
	dcb.Parity=parity;       //У�� 
    dcb.StopBits=stopbit;   //ֹͣλ
    dcb.fDtrControl= dtr;//DTR_CONTROL_DISABLE;
    dcb.fOutxCtsFlow=false;
    dcb.fOutxDsrFlow=false;
    dcb.fRtsControl= rts;//RTS_CONTROL_DISABLE;
    dcb.fAbortOnError=false;
    dcb.fOutX=false;
    dcb.fInX=false; 
    if(!SetCommState(hCom,&dcb)) return FALSE;

    if(!GetCommTimeouts(hCom,&TimeOut)) return FALSE;   
    TimeOut.ReadIntervalTimeout=MAXDWORD;          //2;
    TimeOut.ReadTotalTimeoutMultiplier=0; //5    
	
    TimeOut.WriteTotalTimeoutMultiplier=1;
    TimeOut.WriteTotalTimeoutConstant=1;    
    if(!SetCommTimeouts(hCom,&TimeOut)) return FALSE;

    if(!PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR)) return FALSE;
    return TRUE;
}

//WriteComm: д�˿ڣ���������Ϊ�����������ָ�룬Ҫд�ֽ�����д���ֽ���
BOOL Comm232::WriteComm(void* lpBuffer,DWORD nToWrite,DWORD* nWritten)
{
	DWORD ErrCode;
	DWORD Written = 0;	
	COMSTAT Stat;

	*nWritten = 0;
    if(!ClearCommError(hCom,&ErrCode,&Stat)) 
		return FALSE;
    
	for(int Cs=0;Cs<10;Cs++)
	{
		WriteFile(hCom,(BYTE*)lpBuffer+*nWritten,nToWrite-*nWritten,&Written,NULL);
		*nWritten+=Written;
		if(*nWritten==nToWrite) break; 
		Sleep(50);
	}
	if(*nWritten==nToWrite) return TRUE;
	else
	{
		TRACE("���ʹ�������ʧ��,�������ֽ�%d��,%s\n",*nWritten,CTime(time(NULL)).Format("%m-%d %H:%M:%S"));
		return FALSE;
	}
}

//ReadComm: ���˿ڣ���������Ϊ�����������ָ�룬Ҫ���ֽ����������ֽ���
BOOL Comm232::ReadComm(void* lpBuffer,DWORD nToRead,DWORD* nRead)
{
	DWORD ErrCode;
	COMSTAT Stat;

	*nRead = 0;
	
    if(ReadFile(hCom,lpBuffer,nToRead,nRead,NULL))	
		return TRUE;
    if(!ClearCommError(hCom,&ErrCode,&Stat)) 
		return FALSE;
    return FALSE;
}

//CloseComm: �ض˿�,����Ϊ���
BOOL Comm232::CloseComm()
{
    return CloseHandle(hCom);
}

//�������ݽ�����
int Comm232::FindBytes(BYTE* pBuf,int FindLen,int Bgn)
{
    int Location = -1;	

    for(int i=Bgn;i<BufLen-FindLen+1;i++)    
	{
	    if(memcmp(&ComBuf[i],pBuf,FindLen)==0)
		{
			Location = i;
			break;
		}    
    }
    if(i==BufLen-FindLen+1)
		return -1;
	else
		return Location;
}

int Comm232::FindBytes(BYTE* pBuf2Fnd,int FindLen, BYTE *pBuf, int nBuf, int Bgn)
{
    int Location = -1;	
    for(int i=Bgn;i < nBuf-FindLen+1; i++)  {  
	    if(memcmp(pBuf + i,pBuf2Fnd,FindLen)==0)
		{
			Location = i;
			break;
		}    
    }
    return Location;
}

BOOL Comm232::SetRecedBgn(BYTE* pReceBgn,int Len)
{
	if(PortOpen) return FALSE;
	else 
	{
		if(Len>MaxHeadLen) 
			Len = MaxHeadLen;
		memmove(ReceBgn,pReceBgn,Len);
       	ReceBgnLen = Len;
		return TRUE;
	}
}

BOOL Comm232::SetReceEnd(BYTE* pReceEnd,int Len)
{
	if(PortOpen) return FALSE;
	else
	{
		if(Len>MaxHeadLen) Len = MaxHeadLen;
		memmove(ReceEnd,pReceEnd,Len);
		ReceEndLen = Len;
		return TRUE;
	}
}

BOOL Comm232::SetSendBgn(BYTE* pSendBgn,int Len)
{
	if(PortOpen) return FALSE;
	else 
	{
		if(Len>MaxHeadLen) Len = MaxHeadLen;
		memmove(SendBgn,pSendBgn,Len);
		SendBgnLen = Len;
		return TRUE;
	}
}

BOOL Comm232::SetSendEnd(BYTE* pSendEnd,int Len)
{
	if(PortOpen) return FALSE;
	else 
	{
		if(Len>MaxHeadLen) Len = MaxHeadLen;
		memmove(SendEnd,pSendEnd,Len);
		SendEndLen = Len;
		return TRUE;
	}
}

void Comm232::GetRecedBgn(BYTE* pReceBgn,int *Len)
{
	*Len = ReceBgnLen;
	memmove(pReceBgn,ReceBgn,ReceBgnLen);
}

void Comm232::GetReceEnd(BYTE* pReceEnd,int *Len)
{
	*Len = ReceEndLen;
	memmove(pReceEnd,ReceEnd,ReceEndLen);
}

void Comm232::GetSendBgn(BYTE* pSendBgn,int *Len)
{
	*Len = SendBgnLen;
	memmove(pSendBgn,SendBgn,SendBgnLen);
}

void Comm232::GetSendEnd(BYTE* pSendEnd,int *Len)
{
	*Len = SendEndLen;
	memmove(pSendEnd,SendEnd,SendEndLen);
}

Comm232::Comm232()                      //ȱʡ����
{
	BYTE rBgn[3] = {0x00,0x55,0xAA};       //����ͷ��β��У�鷽ʽ��������9600
	BYTE rEnd[3] = {0xBB,0xCC,0xDD};
	BYTE sBgn[3] = {0x00,0x55,0xAA};
	BYTE sEnd[3] = {0xBB,0xCC,0xDD};
	ReceBgnLen = 3;
	ReceEndLen = 3;
	SendBgnLen = 3;
	SendEndLen = 3;

	memmove(ReceBgn,rBgn,ReceBgnLen);
	memmove(ReceEnd,rEnd,ReceEndLen);
	memmove(SendBgn,sBgn,SendBgnLen);
	memmove(SendEnd,sEnd,SendEndLen);

	BgnPos = -1;
	EndPos = -1;
	BufLen = 0;

	hCom = INVALID_HANDLE_VALUE;
	PortNum = 1;
	bytesize = 8;
	BaudRate = 9600;
    stopbit = ONESTOPBIT;
    parity = 0;
	PortOpen = FALSE;
	Jylx = jcrc;
	rts = RTS_CONTROL_DISABLE;
	dtr = DTR_CONTROL_DISABLE;
}
	
Comm232::~Comm232()
{
	if(PortOpen)
	{
		CloseComm();
		PortOpen = FALSE;
	}
}

BOOL Comm232::SetJyLx(TJylx Lx)
{
	if(PortOpen) return FALSE;
	else
	{
		Jylx = Lx;
		return TRUE;
	}
}

TJylx Comm232::GetJylx()
{
	return Jylx;
}

BOOL Comm232::PortIsOpen()
{
	return PortOpen;
}

BOOL Comm232::SetPara(int ComNum,int Baud,int par,int bytes,int stop)
{
	if(PortOpen) 
		return FALSE;
	else
	{
		PortNum = ComNum;
		BaudRate = Baud;
		parity=par;
	    bytesize=bytes;
		//stopbit = stop;
		return TRUE;
	}
}

BOOL Comm232::SetPortNum(int ComNum)
{
	if(PortOpen) 
		return FALSE;
	else
	{
		PortNum = ComNum;
		return TRUE;
	}
}

void Comm232::GetPara(int* ComNum,int *Baud)
{
	*ComNum = PortNum;
	*Baud = BaudRate;
}

BOOL Comm232::Open()
{
	if(!PortOpen) PortOpen = InitComm();
	return PortOpen;
}
	
BOOL Comm232::Close()
{
	if(PortOpen) CloseComm();
	PortOpen = FALSE;
	return TRUE;
}

BOOL Comm232::ReceData(BYTE* ReceBuf,int Len,int *ReceLen)
{	
	DWORD nToRead = MaxBufLen - BufLen,nRead=0;
	*ReceLen = 0;

	if(PortOpen)
	{
		if(nToRead<MaxBufLen/2)
		{
			BufLen = 0;
			nToRead = MaxBufLen;
			BgnPos = -1;
			EndPos = -1;
		}

		if(ReadComm(&ComBuf[BufLen],nToRead,&nRead)) 
			BufLen+=nRead;
		if(BufLen>0)
		{
			if(BgnPos<0)
			{
				BgnPos = FindBytes(ReceBgn,ReceBgnLen,0);
			}
			if(BgnPos>=0) 
				EndPos = FindBytes(ReceEnd,ReceEndLen,BgnPos+ReceBgnLen);
			else EndPos = -1;
			
			if(BgnPos >= 0 && EndPos > 0)
			{
				if(CRCCheck(&ComBuf[BgnPos+ReceBgnLen],EndPos-BgnPos-ReceBgnLen))           //CRC����У�顣��ˣ���Ҫ���ֽ���ǰ�����ֽ��ں�
				{
                    if(Jylx==jno)  *ReceLen = EndPos-BgnPos-ReceBgnLen-2;
                    if(Jylx==jcrc) *ReceLen = EndPos-BgnPos-ReceBgnLen-2-2;
                    if(Jylx==jbte) *ReceLen = EndPos-BgnPos-ReceBgnLen-1-2;
                    if(Jylx==jdbte)*ReceLen = EndPos-BgnPos-ReceBgnLen-2-2;
					if(*ReceLen>Len) 
						*ReceLen = Len;
					if(*ReceLen>0) 
					{
						memmove(ReceBuf,&ComBuf[BgnPos+ReceBgnLen+sizeof(WORD)],*ReceLen);
						BufLen -= EndPos+ReceEndLen;
						memmove(ComBuf,&ComBuf[EndPos+ReceEndLen],BufLen);
						BgnPos = -1;
						EndPos = -1;
						return TRUE;
					}
					else
					{
						BufLen -= EndPos+ReceEndLen;
						memmove(ComBuf,&ComBuf[EndPos+ReceEndLen],BufLen);
						BgnPos = -1;
						EndPos = -1;
						return FALSE;
					}
				}
				else
				{
					BufLen -= EndPos+ReceEndLen;
					memmove(ComBuf,&ComBuf[EndPos+ReceEndLen],BufLen);
					BgnPos = -1;
					EndPos = -1;
					return FALSE;
				}
			}
			else if(BgnPos<0)
			{
				if(BufLen>ReceBgnLen)
				{
					memmove(ComBuf,&ComBuf[BufLen-ReceBgnLen+1],ReceBgnLen-1);
					BufLen = ReceBgnLen-1;
				}				
				BgnPos = -1;
				EndPos = -1;
				return FALSE;
			}
			else
			{
				if(BgnPos>0)
				{
					memmove(ComBuf,&ComBuf[BgnPos],BufLen-BgnPos);
					BufLen-= BgnPos;
					BgnPos = 0;
					EndPos = -1;
				}
				return FALSE;
			}
		}
	}
	
	return FALSE;
}
/// ֱ�Ӷ����ݵ����� pRecBuf ��
BOOL Comm232::ReceData (DWORD &nRead, BYTE *pRecBuf, int nLen /* =1024 */)
{
	if(PortOpen)
	{
		if(ReadComm (pRecBuf, nLen,&nRead)) 
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL Comm232::SendData(BYTE* SendBuf,int Len)
{
	BYTE TmpBuf[MaxBufLen];
	memset(TmpBuf,0x00,sizeof(TmpBuf));
	DWORD nWrite = 0;
    DWORD Leng = 0;
	if(!PortOpen) 
		return FALSE;

	if(Len>MaxBufLen-ReceBgnLen-ReceEndLen-2)
		Len = MaxBufLen-ReceBgnLen-ReceEndLen-2;
	
	memmove(TmpBuf,SendBgn,SendBgnLen);
	//��ӳ���
	*(WORD *)&TmpBuf[SendBgnLen] = Len;
	memmove(&TmpBuf[SendBgnLen+sizeof(WORD)],SendBuf,Len);
	if(Jylx == jno)
	{
		memmove(&TmpBuf[SendBgnLen+sizeof(WORD)+Len],SendEnd,SendEndLen);
		Leng = Len+SendBgnLen+SendEndLen+2;
	}
	else if(Jylx == jcrc)
	{
		WORD C = CRC(&TmpBuf[SendBgnLen],Len+sizeof(WORD));
		TmpBuf[SendBgnLen+sizeof(WORD)+Len] = C/256;                //���ֽ���ǰ
		TmpBuf[SendBgnLen+sizeof(WORD)+Len+1] = C%256;
		memmove(&TmpBuf[SendBgnLen+sizeof(WORD)+Len+2],SendEnd,SendEndLen);
		Leng = Len+SendBgnLen+SendEndLen+sizeof(WORD)+2;
	}
	else if(Jylx == jbte)
	{
		BYTE C = 0;
		for(int i=0;i<Len+2;i=i++)
		{
			C=C+TmpBuf[SendBgnLen+i]; 
		}
		TmpBuf[SendBgnLen+sizeof(WORD)+Len] = C;                
		memmove(&TmpBuf[SendBgnLen+sizeof(WORD)+Len+1],SendEnd,SendEndLen);
		Leng = Len+SendBgnLen+SendEndLen+sizeof(WORD)+1;
	}
	else if(Jylx == jdbte)
	{
		WORD C = 0;
		for(int i=0;i<Len+2;i=i++)
		{
			C=C+TmpBuf[SendBgnLen+i]; 
		}
		*(WORD *)&TmpBuf[SendBgnLen+sizeof(WORD)+Len] = C;                
		memmove(&TmpBuf[SendBgnLen+sizeof(WORD)+Len+2],SendEnd,SendEndLen);
		Leng = Len+SendBgnLen+SendEndLen+sizeof(WORD)+2;
	}
	WriteComm(TmpBuf,Leng,&nWrite);

	return ( Leng == nWrite);
}

//��ս��ջ�����
void Comm232::PurgeComRcvBuf()
{
  ::PurgeComm(hCom,PURGE_RXCLEAR);
  BufLen=0;
  memset(ComBuf,0x00,sizeof(ComBuf));
}

void Comm232::PurgeComSndBuf()
{
	::PurgeComm(hCom,PURGE_TXCLEAR);
}

BOOL Comm232::SetRts(int iRts)
{
	rts = iRts;
	return TRUE;
}

BOOL Comm232::SetDtr(int iDtr)
{
	dtr = iDtr;
	return TRUE;
}

BOOL Comm232::SetStopBit(int iStop)
{
	stopbit = iStop;
	return TRUE;
}
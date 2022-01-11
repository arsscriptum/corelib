#pragma once

#include <iostream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>   // defines std::getline(std::istream, std::string)
#include <vector>
#include <string>

#define BUFFER_PIPE_SIZE 1024
class NamedPipe
{
public:
	enum IDENTITY
	{
		BLANK,
		SERVER,	//������
		CLIENT,	//�ͻ���
		RCVCLIENT,	//����˽��տͻ���
	};
	NamedPipe(void);
	NamedPipe(HANDLE hPipe);
	~NamedPipe(void);
public:
	const CAtlString GetLastMsg();
	bool connect(LPCTSTR named);
	bool open(LPCTSTR named);
	bool ReadLine(std::string& outBuf);
	bool WriteBytes(const char* buf,size_t size);
	void Flush();
	void close();
	NamedPipe* WaitClient(unsigned int timeout);
 	bool IsClosed();
 	bool IsAlive();
private:
	char mRcvData[BUFFER_PIPE_SIZE];
	HANDLE mPipe;
	bool mClosed;
	IDENTITY mIdentity;
	CAtlString mLastMsg;
	CAtlString mNamed;
	OVERLAPPED mReadEvent;
	OVERLAPPED mWriteEvent;
};
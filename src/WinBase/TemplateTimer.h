#pragma once
#include <atlbase.h>
static void CALLBACK TimerProc(void*, BOOLEAN);
//
//   �����ƣ�CTimer
//   ����������ʱ����
//
class CTimer
{
public:
    CTimer()
    {
        m_hTimer = NULL;
        m_mutexCount = 0;
    }
	virtual ~CTimer()
    {
        Stop();
    }
	
    bool Start(unsigned int nInterval,   // �Ժ���Ϊ��λ
               bool bImmediately = false,// true:����ִ��
               bool bOnce = false)       // true:ִֻ��һ��
    {
        if( m_hTimer )
        {
            return false;
        }

        SetCount(0);
        BOOL bSuccess = CreateTimerQueueTimer( &m_hTimer,
                                              NULL,
											  TimerProc,
                                              this,
                                              bImmediately ? 0 : nInterval,
                                              bOnce ? 0 : nInterval,
											  WT_EXECUTEINTIMERTHREAD);
        return( bSuccess != FALSE );
    }

    void Stop()
    {
		//fixed by jiayh
		//If this parameter is INVALID_HANDLE_VALUE, the function waits for any running timer callback functions to complete before returning.
        if(m_hTimer)
			DeleteTimerQueueTimer( NULL, m_hTimer, INVALID_HANDLE_VALUE );
        m_hTimer = NULL ;
    }
	//��ʱ���¼�
    virtual void OnTimedEvent()
    {
        
    }
	//���ô���
    void SetCount(int value)
    {
        InterlockedExchange( &m_mutexCount, value );
    }
	//��ȡ���ô���
    int GetCount()
    {
        return InterlockedExchangeAdd( &m_mutexCount, 0 );
    }
	operator HANDLE()
	{
		return m_hTimer;
	}
private:
    HANDLE m_hTimer;
    long m_mutexCount;
	bool m_bCall;
};

//��ʱ���ص�����
void CALLBACK TimerProc(void* param, BOOLEAN timerCalled)
{
    CTimer* timer = static_cast<CTimer*>(param);
    timer->SetCount( timer->GetCount()+1 );
    timer->OnTimedEvent();
};

//
//   �����ƣ�CTemplateTimer
//   ����������ʱ��ģ����
//
template <class T> class CTemplateTimer : public CTimer
{
public:
    typedef private void (T::*TimedFunction)(void);

    CTemplateTimer()
    {
        m_pTimedFunction = NULL;
        m_pClass = NULL;
    }

    void SetTimedEvent(T *pClass, TimedFunction pFunc)
    {
        m_pClass         = pClass;
        m_pTimedFunction = pFunc;
    }

protected:
    void OnTimedEvent()  
    {
        if (m_pTimedFunction && m_pClass)
        {
            (m_pClass->*m_pTimedFunction)();
        }
    }

private:
    T *m_pClass;
    TimedFunction m_pTimedFunction;
};

template <class T>
HANDLE CreateWaitTimer(DWORD dwInterval)
{
	HANDLE hRet = ::CreateWaitableTimer(NULL,FALSE,NULL);
	if(hRet==NULL)
		return NULL;
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -10000*(__int64)dwInterval;
	BOOL bRet = SetWaitableTimer(hRet,&liDueTime, dwInterval,NULL,NULL,TRUE);
	if (!bRet)
	{
		CloseHandle(hRet);
		return NULL;
	}
	return hRet;
}
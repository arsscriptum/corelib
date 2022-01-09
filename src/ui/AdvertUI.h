#pragma once

//���ؼ���������ʾ�����Ϣ��֧�ֶ�̬�л��Լ�����¼�
namespace CustomUI
{
class Advert : public DuiLib::CVerticalLayoutUI
{
public:
	Advert(void);
	~Advert(void);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetNormalImage(LPCTSTR pName);
	void SetHotImage(LPCTSTR pName);
	void SetSelectedImage(LPCTSTR pName);
	void SetInterruptTimer(LONG ltm);
	void StopInterruptTimer(void);
	void AppendAdvertImage(LPCTSTR pFile);
	void SetGroup(LPCTSTR pGroup);
	bool SelectItem(int nIndex);
	LPCTSTR GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);
	int	 ReloadRes(LPCTSTR pDir);
	void SetPos(RECT rc);
	void NextPage();
	void FrontPage();
	void SetManager(DuiLib::CPaintManagerUI* pManager,DuiLib::CControlUI* pParent, bool bInit = true);
	DuiLib::CControlUI* FindControl(DuiLib::FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
	int  GetPageNo();
	DuiLib::CEventSource OnChangePage;
protected:
	void DoPaint(HDC hDC, const RECT& rcPaint);

	void DoEvent(DuiLib::TEventUI& event);
	bool OnOptionEvent( void *param );
	bool OnShowAreaEvent(void *param);
	void UpdateOptions(const SIZE &sz);
private:
	bool						m_bStop;
	long						m_timer;
	bool						m_hasResizeShow;//��Ҫ���¼�����ʾ���߶�
	bool						m_hasResizeBar;
	SIZE						m_szOption;		//��ť��Ⱥ͸߶�
	DuiLib::CDuiString			m_pichot;
	DuiLib::CDuiString			m_picnormal;
	DuiLib::CDuiString			m_picselected;
	DuiLib::CDuiString			m_picdir;		//���ͼƬĿ¼

	DuiLib::CDuiString			m_optGroup;
	DuiLib::CTabLayoutUI		*m_lstAdvert;	//����б�
	DuiLib::CHorizontalLayoutUI *m_lstBtns;		//��ť����

	bool m_bHaltOnMouse;
};
}
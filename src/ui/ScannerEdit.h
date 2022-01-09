#pragma once

#include "../base/MacroX.h"
namespace CustomUI
{
	class ScannerEditUI : public DuiLib::CRichEditUI
	{
	public:
		ScannerEditUI(void);
		~ScannerEditUI(void);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetBkTextColor(DWORD dwColor);
		void SetBkFont(int nI);
		void SetBkTextPadding(RECT rc);
		void SetText(LPCTSTR pstrValue);
		void SetNumberOnly(bool bNumberOnly);
		bool IsNumberOnly();
		const DuiLib::CDuiString& GetScanText() const;
		DuiLib::CEventSource OnScanComplete;
		DuiLib::CEventSource OnEnter;
		DuiLib::CEventSource OnInputEvent;
		DuiLib::CEventSource OnLostFocus;
		DuiLib::CEventSource OnHaveFocus;
		DUI_DECLARE(CustomUI::ScannerEditUI,DuiLib::CRichEditUI)
	protected:
		void Init();
		void EnaleMultiLine(bool b);
		bool OnScanInput(void *param);
		void DoEvent(DuiLib::TEventUI& event);
		void PaintText(HDC hDC);
		void PaintBorder(HDC hDC);
		HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const;
	protected:
		HIMC  m_imc;
		DuiLib::CDuiString m_scanText; //���������
		DuiLib::CDuiString m_bkText;//�����ı�
		DWORD m_bkTextColor;		//�����ı���ɫ
		RECT  m_bkTextPadding;		//�����ı�λ��
		int	  m_bkFont;				//�����ı�����
		UINT  m_bkTextStyle;		//�����ı���ʽ
		bool  m_needbktext;
		bool  m_keydown;			//���̰���״̬
		bool  m_numberOnly;			//ֻ��������������
	};
}
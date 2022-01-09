#pragma once

#include "../base/MacroX.h"
namespace CustomUI
{
	class EditUI : public DuiLib::CRichEditUI 
	{
	public:
		EditUI(void);
		~EditUI(void);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetBkTextColor(DWORD dwColor);
		void SetBkFont(int nI);
		void SetBkTextPadding(RECT rc);
		void SetText(LPCTSTR pstrValue);
		void SetBkText(LPCTSTR pstrValue);
		bool IsPasswordMode();
		bool IsMultiLine();
		void EnaleMultiLine(bool b);
		void EnableIME(bool b);
		virtual void Init();
		DuiLib::CEventSource OnInputEvent;
		DuiLib::CEventSource OnInputFilter;
		DUI_DECLARE(CustomUI::EditUI,DuiLib::CRichEditUI)
	protected:
		void OnTxNotify(DWORD iNotify, void *pv);
		void DoEvent(DuiLib::TEventUI& event);
		void PaintText(HDC hDC);
		void PaintBorder(HDC hDC);
		HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const;
	protected:
		CAtlString tmpPreBuffer;
		DuiLib::CDuiString m_bkText;//�����ı�
		DWORD m_bkTextColor;		//�����ı���ɫ
		RECT  m_bkTextPadding;		//�����ı�λ��
		int	  m_bkFont;				//�����ı�����
		UINT  m_bkTextStyle;		//�����ı���ʽ
		bool  m_needbktext;
		bool  m_bIME;				//�Ƿ��������뷨
		HIMC  m_imc;
	};
}

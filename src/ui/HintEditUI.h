#pragma once

#include "../base/MacroX.h"
namespace CustomUI
{
	class HintEditUI : public DuiLib::CEditUI
	{
	public:
		HintEditUI(void);
		~HintEditUI(void);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void SetBkTextColor(DWORD dwColor);
		void SetBkFont(int nI);
		void SetBkTextPadding(RECT rc);
		void SetText(LPCTSTR pstrValue);
		void SetBkText(LPCTSTR pstrValue);
		bool IsMultiLine();
		void EnaleMultiLine(bool b);
		void EnableIME(bool b);
		virtual void Init();
		DuiLib::CEventSource OnInputEvent;
		DuiLib::CEventSource OnInputFilter;
		DUI_DECLARE(CustomUI::HintEditUI,DuiLib::CEditUI)
	protected:
 		void DoEvent(DuiLib::TEventUI& event);
		void PaintText(HDC hDC);
		void PaintBorder(HDC hDC);
 	protected:
		CAtlString tmpPreBuffer;
		DuiLib::CDuiString m_bkText;//�����ı�
		DWORD m_bkTextColor;		//�����ı���ɫ
		RECT  m_bkTextPadding;		//�����ı�λ��
		int	  m_bkFont;				//�����ı�����
		UINT  m_bkTextStyle;		//�����ı���ʽ
		bool  m_needbktext;
 	};
}

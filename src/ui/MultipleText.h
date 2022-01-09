#pragma once

#include "MacroX.h"
namespace CustomUI
{
	class MultipleText : public DuiLib::CTextUI
	{
	public:
		MultipleText();
		void SetText(LPCTSTR pstrText);
		void CalculationsDrawText(HDC hDC,RECT &rc,DuiLib::CDuiString &outStr,int nFont);
		
		void SetLink(bool bV);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoEvent(DuiLib::TEventUI& event);
		bool Activate();
		SIZE EstimateSize(SIZE szAvailable);
		RECT GetTextRc();
		DUI_DECLARE(CustomUI::MultipleText,DuiLib::CTextUI)
	protected:
		void PaintText(HDC hDC);
	protected:
		DuiLib::CDuiString m_drawText;		//����
		bool		m_bShowAll;
		bool		m_bLink;		//�����ӷ�ʽ��ʾ	
		RECT		m_rcText;		//�ı�����
		UINT		m_unState;		//�ı�����״̬
		HCURSOR		m_cursortmp;	//���ָ����ʱ����
	};
}
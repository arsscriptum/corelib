#pragma once

#include "../Base/MacroX.h"
namespace CustomUI
{
	class ListUI : public DuiLib::CListUI
	{
	public:
		ListUI(void);
		//���� ���ȼ���listbody item������ʱ��û�м���Column����listbody item pos���㲻��ȷ
		void SetPos(RECT rc);
		void ScrollAt(DuiLib::CControlUI *item);
		void SetBkImage(LPCTSTR pStrImage);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		DUI_DECLARE(CustomUI::ListUI,DuiLib::CListUI)
	protected:
		void PaintBkImage(HDC hDC);
		RECT GetImgDest(LPCTSTR img);
		void RatioW(RECT rc,double inW,double &outW);
		void RatioH(RECT rc,double inH,double &outH);
		void ReBkImgDest(RECT rc);
	protected:
		RECT rcBkImg;	//����ͼ�����Ҽ��ԭʼ���λ�úͿ��
	};
	//
	class ListHeaderItemUI : public DuiLib::CListHeaderItemUI
	{
	public:
		ListHeaderItemUI();
		void SetOwner(DuiLib::CListUI *listUI);
		bool Add(DuiLib::CControlUI* pControl);
		SIZE EstimateSize(SIZE szAvailable);
		DUI_DECLARE(CustomUI::ListHeaderItemUI,DuiLib::CListHeaderItemUI)
	protected:
		void DoEvent(DuiLib::TEventUI& event);
		void Init();
		void LoopSetOwner(DuiLib::CControlUI* pControl);
		void SetChildrenOwner();
	private:
		DuiLib::CListUI *mList;
		SIZE mAvailableSize;
		SIZE mEstSize;	//�����Ŀ��ֵ
		float  mRatX;
	};
	//
	class CListHeaderUI : public DuiLib::CListHeaderUI
	{
	public:
		CListHeaderUI();
		bool Add(DuiLib::CControlUI* pControl);
		SIZE EstimateSize(SIZE szAvailable);
		DuiLib::CListUI* GetListUI();
		DUI_DECLARE(CustomUI::CListHeaderUI,DuiLib::CListHeaderUI)
	protected:
		typedef struct {
			DuiLib::CControlUI* ui;
			SIZE sz;
		}NEED_SIZE;
		void Init();
		long SubCtrSizeForRaw();
		void GetValidCtrForRaw(std::vector<NEED_SIZE> &out,SIZE szBlank);
		SIZE InsideEstimateSize(SIZE szAvailable);
	private:
		//��¼ÿ���ؼ�ԭʼ��С
		std::map<DuiLib::CControlUI*,SIZE> mCtrRawSize;
		SIZE szUpSize;
		DuiLib::CDuiRect rcNowList;
	};
}
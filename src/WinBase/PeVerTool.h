#pragma once;
#include <map>
#include <vector>
#include <atlstr.h>
namespace base{
	class PeInfoVal
	{
	public:
		PeInfoVal(){	}
		virtual ~PeInfoVal();
		//���Ա���
		struct CodePage
		{
			WORD wLanguage;
			WORD wCodePage;
		};	 
		typedef std::map<CAtlString,CAtlString> Values;
		void Set(void* data);
		bool IsSuc();
		// Accessors to the different version properties.
		// Returns an empty string if the property is not found.
		CAtlString company_name();
		CAtlString company_short_name();
		CAtlString product_name();
		CAtlString product_short_name();
		CAtlString internal_name();
		CAtlString product_version() ;
		CAtlString private_build() ;
		CAtlString special_build() ;
		CAtlString comments() ;
		CAtlString original_filename() ;
		CAtlString file_description() ;
		CAtlString file_version() ;
		CAtlString legal_copyright() ;
		CAtlString legal_trademarks() ;
		CAtlString last_change() ;
		bool is_official_build() ;
	protected:
		// Get the fixed file info if it exists. Otherwise NULL
		const VS_FIXEDFILEINFO& fixed_file_info() { return fixed_file_info_; }
		CAtlString GetValue(LPCTSTR name,void* data);
	private:
		Values data_;
		int language_;
		int code_page_;
		VS_FIXEDFILEINFO fixed_file_info_;
	};
	
	typedef struct SignInfo
	{
		CAtlString author;	//ǩ��������
		CAtlString timestamp;	//ǩ��ʱ��
		CAtlString programName;
		CAtlString publisherLink;
		CAtlString moreInfoLink;
	}SIGN_INFO;
	
	PeInfoVal GetPeInfo(PCTSTR pcszFileName);
	CAtlString GetSelfVersion(HMODULE hModule=NULL);
	int CompareVersion(LPCTSTR a,LPCTSTR b);
	
	/*
		@input
			pcszFileName ������ǩ���ļ�����·��
		@return
			SIGN_INFO ��ȡǩ����Ϣ
	*/
	SIGN_INFO GetSoftSign(PCTSTR pcszFileName);
}
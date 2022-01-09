#include "StdAfx.h"

#include "TiXmlDocEx.h"

TiXmlDocumentEx::TiXmlDocumentEx(void)
{
}

TiXmlDocumentEx::~TiXmlDocumentEx(void)
{
}
bool TiXmlDocumentEx::GetNodePointerByName(TiXmlElement* pRootEle, string strNodeName,TiXmlElement* &Node)   
{   
	// ������ڸ��ڵ��������˳�   
	if (strNodeName==pRootEle->Value())   
	{   
		Node = pRootEle;   
		return true;   
	}   
	TiXmlElement* pEle = pRootEle;     
	for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())     
	{     
		//�ݹ鴦���ӽڵ㣬��ȡ�ڵ�ָ��   
		if(GetNodePointerByName(pEle,strNodeName,Node))   
			return true;   
	}     
	return false;   
}  
bool TiXmlDocumentEx::QueryNode_Text(string strNodeName, string &strText)   
{   

	TiXmlElement *pRootEle = RootElement();   
	if (NULL==pRootEle)   
	{   
		return false;   
	}   
	TiXmlElement *pNode = NULL;   
	GetNodePointerByName(pRootEle,strNodeName,pNode);   
	if (NULL!=pNode)   
	{   
		const char *pText = pNode->GetText();
		if(pText)
		{
			strText = pText;
		}
		else
		{
			strText = "";
		}
		
		return true;   
	}   
	else  
	{   
		return false;   
	}   

}   

bool TiXmlDocumentEx::QueryNode_Attribute(string strNodeName, map<string, string> &AttMap)   
{   
	TiXmlElement *pNode = NULL;   
	GetNodePointerByName(RootElement(), strNodeName,pNode);   
	if (NULL!=pNode)   
	{   
		TiXmlAttribute* pAttr = NULL;    
		for (pAttr = pNode->FirstAttribute(); pAttr; pAttr = pAttr->Next())     
		{     
			std::string strAttName = pAttr->Name();   
			std::string strAttValue = pAttr->Value();   
			AttMap.insert(map<string, string>::value_type(strAttName,strAttValue));   
		}     
		return true;   
	}   
	else  
	{   
		return false;   
	}   
	return true;   
} 


bool TiXmlDocumentEx::DelNode(string strNodeName)   
{   

	TiXmlElement *pRootEle = RootElement();   
	if (NULL==pRootEle)   
	{   
		return false;   
	}   
	TiXmlElement *pNode = NULL;   
	GetNodePointerByName(RootElement(), strNodeName, pNode);   
	// �����Ǹ��ڵ�   
	if (pRootEle==pNode)   
	{   
		if(RemoveChild(pRootEle))   
		{   
			SaveFile();
			return true;   
		}   
		else    
			return false;   
	}   
	// �����������ڵ�   
	if (NULL!=pNode)   
	{   
		TiXmlNode *pParNode =  pNode->Parent();   
		if (NULL==pParNode)   
		{   
			return false;   
		}   

		TiXmlElement* pParentEle = pParNode->ToElement();   
		if (NULL!=pParentEle)   
		{   
			if(pParentEle->RemoveChild(pNode))   
				SaveFile();   
			else  
				return false;   
		}   
	}   
	else  
	{   
		return false;   
	}   
	return false;   
} 
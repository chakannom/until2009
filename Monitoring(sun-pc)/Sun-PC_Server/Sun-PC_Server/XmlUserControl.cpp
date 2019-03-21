#include "XmlUserControl.h"

//COM��ü �ʱ�ȭ
BOOL InitializeUserCOM() {
// ������ ���� ������ �ö󰡸鼭 Ȯ��� �ʱ�ȭ �Լ��� ��� �� �� �ִ�.
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	CoInitialize(NULL);
#endif
	return TRUE;
}
//IXMLDOMDocument2 �ʱ�ȭ
BOOL InitializeUserMSXML() {

	pXDoc2User.CreateInstance(__uuidof(MSXML2::DOMDocument));  //Instance ����
	pXDoc2User->async=VARIANT_FALSE;
	pXDoc2User->validateOnParse=VARIANT_TRUE;
	pXDoc2User->resolveExternals=VARIANT_TRUE;

	return TRUE;
}

BOOL XmlUserInit() {
	BOOL Ret;
	string dir = "user\\";

	CreateDir((char*)dir.c_str());

	FileName = dir+"user.xml";

	InitializeUserCOM();
	InitializeUserMSXML();

	Ret=pXDoc2User->load((_variant_t)FileName.c_str());
	if(Ret==FALSE) {
		MSXML2::IXMLDOMProcessingInstructionPtr pXPI;  //IXMLDOMProcessingInstruction ����(<? �� ?> �±� ������ ���� ���)
		MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement ����(Element�� ��Ÿ��)

		pXPI=pXDoc2User->createProcessingInstruction(L"SUN_USER_XML",L"version=\"1.0\" encoding=\"euc-kr\"");  //PI ����
		pXElement=pXDoc2User->createElement(L"List");  //List Element ����
		pXDoc2User->appendChild(pXPI);  //pXDoc2 �ؿ� pXPI�� �߰�
		pXDoc2User->appendChild(pXElement);  //pXDoc2 �ؿ� pXElement�� �߰�
		pXDoc2User->save((_variant_t)FileName.c_str());
		
		pXElement.Release();
		pXPI.Release();
	}

	return TRUE;
}

BOOL XmlUserInsert(const string& UserID, const string& UserPW) {
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement ����(Element�� ��Ÿ��)

	string findID="//*[@ID=\'"+UserID+"\']";

	pXElement=pXDoc2User->selectSingleNode(findID.c_str());
	if(pXElement!=NULL) {
		pXElement.Release();
		return FALSE;
	}

	pXElement=pXDoc2User->createElement(L"User");  //User Element ����
	pXElement->setAttribute(L"ID",UserID.c_str());  //PW �߰�
	pXElement->setAttribute(L"PW",UserPW.c_str());  //PW �߰�
	pXDoc2User->GetlastChild()->appendChild(pXElement);  //pXElement[0] �ؿ� pXElement[1]�� �߰�

	pXDoc2User->save((_variant_t)FileName.c_str());

	pXElement.Release();

	return TRUE;
}

BOOL XmlUserModify(const string& UserID, const string& UserPW) {
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement ����(Element�� ��Ÿ��)

	string findID="//*[@ID=\'"+UserID+"\']";

	pXElement=pXDoc2User->selectSingleNode(findID.c_str());
	if(pXElement==NULL) return FALSE;

	pXElement->setAttribute(L"PW",UserPW.c_str());

	pXDoc2User->save((_variant_t)FileName.c_str());

	pXElement.Release();

	return TRUE;
}
BOOL XmlUserDelete(const string& UserID, const string& UserPW) {
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement ����(Element�� ��Ÿ��)

	string findID="//*[@ID=\'"+UserID+"\']";

	pXElement=pXDoc2User->selectSingleNode(findID.c_str());
	if(pXElement==NULL) return FALSE;
	if(UserPW!=ConverseBstrToString(pXElement->getAttribute(L"PW").bstrVal)) {
		pXElement.Release();
		return FALSE;
	}

	pXDoc2User->GetlastChild()->removeChild(pXElement);

	pXDoc2User->save((_variant_t)FileName.c_str());

	pXElement.Release();

	return TRUE;
}

BOOL XmlUserSearch(const string& UserID, const string& UserPW) {
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement ����(Element�� ��Ÿ��)

	string findID="//*[@ID=\'"+UserID+"\']";

	pXElement=pXDoc2User->selectSingleNode(findID.c_str());
	if(pXElement==NULL) return FALSE;
	if(UserPW!=ConverseBstrToString(pXElement->getAttribute(L"PW").bstrVal)) {
		pXElement.Release();	
		return FALSE;
	}

	pXElement.Release();

	return TRUE;
}

BOOL XmlUserRelease() {

	pXDoc2User.Release();

	return TRUE;
}

void XmlUserListReview(HWND hList) {
	ListView_DeleteAllItems(hList);
	MSXML2::IXMLDOMNodeListPtr pXNodeList;  //IXMLDOMNodeList ����
	MSXML2::IXMLDOMNamedNodeMapPtr pXNameNodeMap;  //IXMLDOMNamedNodeMap ����

	LVITEM lvitem;
	string tmp;

	pXNodeList=pXDoc2User->selectNodes(L"//User"); 
	for(int i=0;i<pXNodeList->Getlength();i++) {
		pXNameNodeMap=pXNodeList->Getitem(i)->Getattributes();

		tmp=ConverseBstrToString(pXNameNodeMap->Getitem(0)->GetnodeValue().bstrVal);
		lvitem.mask=LVIF_TEXT|LVIF_PARAM;
		lvitem.iItem=i;
		lvitem.iSubItem=0;
		lvitem.pszText=(LPSTR)tmp.c_str();
		ListView_InsertItem(hList,&lvitem);

		tmp=ConverseBstrToString(pXNameNodeMap->Getitem(1)->GetnodeValue().bstrVal);
		ListView_SetItemText(hList,i,1,(LPSTR)tmp.c_str());
	}
	ListView_SetExtendedListViewStyle(hList,LVS_EX_FULLROWSELECT);
}

string ConverseBstrToString(const _bstr_t& str) {
	USES_CONVERSION;

	return std::string(W2A(str));
}

void CreateDir(char* Path) {
	char DirName[256];
	char* p = Path;
	char* q = DirName;   

	while(*p) {
		if (('\\' == *p) || ('/' == *p)) {
			if (':' != *(p-1)) {
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = '\0';
	}
	CreateDirectory(DirName, NULL);
}
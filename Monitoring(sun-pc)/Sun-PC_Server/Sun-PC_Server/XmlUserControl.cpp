#include "XmlUserControl.h"

//COM객체 초기화
BOOL InitializeUserCOM() {
// 윈도우 버젼 버젼이 올라가면서 확장된 초기화 함수를 사용 할 수 있다.
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	CoInitialize(NULL);
#endif
	return TRUE;
}
//IXMLDOMDocument2 초기화
BOOL InitializeUserMSXML() {

	pXDoc2User.CreateInstance(__uuidof(MSXML2::DOMDocument));  //Instance 생성
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
		MSXML2::IXMLDOMProcessingInstructionPtr pXPI;  //IXMLDOMProcessingInstruction 선언(<? 와 ?> 태그 사이의 값을 담당)
		MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement 선언(Element를 나타냄)

		pXPI=pXDoc2User->createProcessingInstruction(L"SUN_USER_XML",L"version=\"1.0\" encoding=\"euc-kr\"");  //PI 생성
		pXElement=pXDoc2User->createElement(L"List");  //List Element 생성
		pXDoc2User->appendChild(pXPI);  //pXDoc2 밑에 pXPI를 추가
		pXDoc2User->appendChild(pXElement);  //pXDoc2 밑에 pXElement를 추가
		pXDoc2User->save((_variant_t)FileName.c_str());
		
		pXElement.Release();
		pXPI.Release();
	}

	return TRUE;
}

BOOL XmlUserInsert(const string& UserID, const string& UserPW) {
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement 선언(Element를 나타냄)

	string findID="//*[@ID=\'"+UserID+"\']";

	pXElement=pXDoc2User->selectSingleNode(findID.c_str());
	if(pXElement!=NULL) {
		pXElement.Release();
		return FALSE;
	}

	pXElement=pXDoc2User->createElement(L"User");  //User Element 생성
	pXElement->setAttribute(L"ID",UserID.c_str());  //PW 추가
	pXElement->setAttribute(L"PW",UserPW.c_str());  //PW 추가
	pXDoc2User->GetlastChild()->appendChild(pXElement);  //pXElement[0] 밑에 pXElement[1]를 추가

	pXDoc2User->save((_variant_t)FileName.c_str());

	pXElement.Release();

	return TRUE;
}

BOOL XmlUserModify(const string& UserID, const string& UserPW) {
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement 선언(Element를 나타냄)

	string findID="//*[@ID=\'"+UserID+"\']";

	pXElement=pXDoc2User->selectSingleNode(findID.c_str());
	if(pXElement==NULL) return FALSE;

	pXElement->setAttribute(L"PW",UserPW.c_str());

	pXDoc2User->save((_variant_t)FileName.c_str());

	pXElement.Release();

	return TRUE;
}
BOOL XmlUserDelete(const string& UserID, const string& UserPW) {
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement 선언(Element를 나타냄)

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
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement 선언(Element를 나타냄)

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
	MSXML2::IXMLDOMNodeListPtr pXNodeList;  //IXMLDOMNodeList 선언
	MSXML2::IXMLDOMNamedNodeMapPtr pXNameNodeMap;  //IXMLDOMNamedNodeMap 선언

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
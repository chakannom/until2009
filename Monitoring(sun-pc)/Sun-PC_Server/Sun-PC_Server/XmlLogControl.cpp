#include "XmlLogControl.h"

CXmlLogControl::CXmlLogControl(void)
{
	InitializeLogMSXML();
}

CXmlLogControl::~CXmlLogControl(void)
{
	pXDoc2Log.Release();
}
//IXMLDOMDocument2 �ʱ�ȭ
BOOL CXmlLogControl::InitializeLogMSXML() {

	pXDoc2Log.CreateInstance(__uuidof(MSXML2::DOMDocument));  //Instance ����
	pXDoc2Log->async = VARIANT_FALSE;
	pXDoc2Log->validateOnParse = VARIANT_TRUE;
	pXDoc2Log->resolveExternals = VARIANT_TRUE;

	return TRUE;
}

BOOL CXmlLogControl::XmlLogLoad(const string userid, const string date, const string time, const string ipaddr) {
	BOOL Ret;
	string dir = "log\\"+userid+"\\"+date+"\\";

	CreateDir((char*)dir.c_str());

	FileName = dir+date+"_"+time+".xml";

	Ret=pXDoc2Log->load((_variant_t)FileName.c_str());

	if(Ret==FALSE) {
		MSXML2::IXMLDOMProcessingInstructionPtr pXPI;  //IXMLDOMProcessingInstruction ����(<? �� ?> �±� ������ ���� ���)
		MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement ����(Element�� ��Ÿ��)

		pXPI=pXDoc2Log->createProcessingInstruction(L"SUN_LOG_XML", L"version=\"1.0\" encoding=\"euc-kr\"");  //PI ����
		pXElement = pXDoc2Log->createElement(L"Log");  //List Element ����
		pXElement->setAttribute(L"UserID", userid.c_str());
		pXElement->setAttribute(L"Date", date.c_str());
		pXElement->setAttribute(L"Time", time.c_str());
		pXElement->setAttribute(L"IP_Address", ipaddr.c_str());
		pXDoc2Log->appendChild(pXPI);  //pXDoc2 �ؿ� pXPI�� �߰�
		pXDoc2Log->appendChild(pXElement);  //pXDoc2 �ؿ� pXElement�� �߰�
		pXDoc2Log->save((_variant_t)FileName.c_str());

		pXElement.Release();
		pXPI.Release();
	}

	return TRUE;
}

BOOL CXmlLogControl::XmlLogInsert(const string time, const string pcssid, const string pcssname, const BOOL bcreate) {
	MSXML2::IXMLDOMElementPtr pXElement;  //IXMLDOMElement ����(Element�� ��Ÿ��)

	if(bcreate) pXElement=pXDoc2Log->createElement(L"Process_Start");
	else pXElement=pXDoc2Log->createElement(L"Process_End");

	pXElement->setAttribute(L"Time", time.c_str());
	pXElement->setAttribute(L"ID", pcssid.c_str());
	pXElement->setAttribute(L"Name", pcssname.c_str());
	pXDoc2Log->GetlastChild()->appendChild(pXElement);  //pXElement[0] �ؿ� pXElement[1]�� �߰�

	pXDoc2Log->save((_variant_t)FileName.c_str());

	pXElement.Release();

	return TRUE;
}

BOOL CXmlLogControl::XmlLogRelease() {

	pXDoc2Log.Release();

	return TRUE;
}

void CXmlLogControl::CreateDir(char* Path) {
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
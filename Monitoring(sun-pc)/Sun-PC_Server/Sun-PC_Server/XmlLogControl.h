#pragma once
#undef UNICODE
#undef _UNICODE

#import <msxml4.dll>  //XML ����
#include <atlconv.h> // USES_CONVERSION�� W2A�� ����ϱ� ���� include
#include <comdef.h>
#include <commctrl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
using namespace std;

class CXmlLogControl
{
public:
	MSXML2::IXMLDOMDocument2Ptr pXDoc2Log;
	string FileName;

public:
	CXmlLogControl(void);
	~CXmlLogControl(void);
	BOOL XmlLogLoad(const string userid, const string date, const string time, const string ipaddr);
	BOOL XmlLogInsert(const string time, const string pcssid, const string pcssname, const BOOL bcreate);
	BOOL XmlLogRelease();

private:
	BOOL InitializeLogMSXML();
	BOOL InitializeLogCOM();
	void CreateDir(char* Path);
};


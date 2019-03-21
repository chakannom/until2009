#undef UNICODE
#undef _UNICODE

#import <msxml4.dll>  //XML 관련
#include <atlconv.h> // USES_CONVERSION과 W2A를 사용하기 위해 include
#include <comdef.h>
#include <commctrl.h>
#include <string>
using namespace std;

static MSXML2::IXMLDOMDocument2Ptr pXDoc2User;  //IXMLDOMDocument2 선언
static string FileName;

BOOL InitializeUserCOM();
BOOL InitializeUserMSXML();
BOOL XmlUserInit();
BOOL XmlUserInsert(const string& UserID, const string& UserPW);
BOOL XmlUserModify(const string& UserID, const string& UserPW);
BOOL XmlUserDelete(const string& UserID, const string& UserPW);
BOOL XmlUserSearch(const string& UserID, const string& UserPW);
BOOL XmlUserRelease();
void XmlUserListReview(HWND hList);
string ConverseBstrToString(const _bstr_t& str);
void CreateDir(char* Path);
#undef UNICODE
#undef _UNICODE

#include <windows.h>

void FileRead(LPCSTR FileName, LPVOID Buffer, DWORD BufLen);
void FileWrite(LPCSTR FileName, LPVOID Buffer, DWORD BufLen);
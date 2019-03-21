#include "FileCtrl.h"

void FileRead(LPCSTR FileName, LPVOID Buffer, DWORD BufLen) {
	RtlZeroMemory(Buffer, sizeof(Buffer));
	DWORD dwRead;
	HANDLE hFile = CreateFile(FileName, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	ReadFile(hFile, Buffer, BufLen, &dwRead, NULL);
	CloseHandle(hFile);
}
void FileWrite(LPCSTR FileName, LPVOID Buffer, DWORD BufLen) {
	DWORD dwWrite;
	HANDLE hFile = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	WriteFile(hFile, Buffer, BufLen, &dwWrite, NULL);
	CloseHandle(hFile);
}
			
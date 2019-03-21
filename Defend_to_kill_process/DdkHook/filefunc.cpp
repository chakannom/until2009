#include "filefunc.h"

//현재 CreateFile이 어떤 스타일의 이름을 사용했는지를 파악
int GetTypeCreate(PFILE_OBJECT FileObject) {
	if(!FileObject->FileName.Length) return NONETYPE;

	if(!memcmp(L"\\PSNUM",FileObject->FileName.Buffer,FileObject->FileName.Length)) return STRINGTYPE;

	return NONETYPE;
}

//CreateFile의 이름스타일에 맞는 구조체를 정의하여 생성
PVOID AllocateStructForFileContext(PFILE_OBJECT FileObject) {
	int Type=GetTypeCreate(FileObject);

	if(Type==NONETYPE) return NULL;

	switch(Type) {
		case STRINGTYPE:
		{
			PCONTEXT_FOR_STRING pContextForString;
			FileObject->FsContext=ExAllocatePool(NonPagedPool,sizeof(CONTEXT_FOR_STRING));
			pContextForString=(PCONTEXT_FOR_STRING)FileObject->FsContext;
			pContextForString->ContextType=STRINGTYPE;
			pContextForString->StringBuffer=(char*)ExAllocatePool(NonPagedPool,MAXSTRINGBUFFERCOUNT+1);
			pContextForString->CurrentRemainBufferSize=0;
			return FileObject->FsContext;
		}
		default:
			return NULL;
	}
}

//사용되었던 FsContext를 위한 메모리를 제거
VOID FreeStructForFileContext(PFILE_OBJECT FileObject) {
	PCONTEXT_FOR_COMMON pContext=(PCONTEXT_FOR_COMMON)FileObject->FsContext;

	if(!pContext) return;

	switch(pContext->ContextType) {
		case STRINGTYPE:
		{
			PCONTEXT_FOR_STRING pContextForString=(PCONTEXT_FOR_STRING)pContext;
			if(pContextForString->StringBuffer) {
				ExFreePool(pContextForString->StringBuffer);
			}
		}
			break;
		default:
			return;
	}
	ExFreePool(pContext);
}
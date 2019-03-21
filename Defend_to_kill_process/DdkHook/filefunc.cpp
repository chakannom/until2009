#include "filefunc.h"

//���� CreateFile�� � ��Ÿ���� �̸��� ����ߴ����� �ľ�
int GetTypeCreate(PFILE_OBJECT FileObject) {
	if(!FileObject->FileName.Length) return NONETYPE;

	if(!memcmp(L"\\PSNUM",FileObject->FileName.Buffer,FileObject->FileName.Length)) return STRINGTYPE;

	return NONETYPE;
}

//CreateFile�� �̸���Ÿ�Ͽ� �´� ����ü�� �����Ͽ� ����
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

//���Ǿ��� FsContext�� ���� �޸𸮸� ����
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
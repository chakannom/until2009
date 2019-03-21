#ifndef FILEFUNC_H_INCLUDE_
#define FILEFUNC_H_INCLUDE_

#ifdef __cplusplus
extern "C" {
#endif 

#include <ntddk.h>

#ifdef __cplusplus
}
#endif

enum {STRINGTYPE=0,NONETYPE};

//Common
typedef struct _CONTEXT_FOR_COMMON {
	int ContextType;  //� Context������ �����ϱ� ���ؼ� �����մϴ�
}CONTEXT_FOR_COMMON,*PCONTEXT_FOR_COMMON;

//for STRING
typedef struct _CONTEXT_FOR_STRING {
	int ContextType;  //� Context������ �����ϱ� ���ؼ� �����մϴ�
#define MAXSTRINGBUFFERCOUNT 100
	char *StringBuffer;
	int CurrentRemainBufferSize;
}CONTEXT_FOR_STRING,*PCONTEXT_FOR_STRING;

int GetTypeCreate(PFILE_OBJECT FileObject);
PVOID AllocateStructForFileContext(PFILE_OBJECT FileObject);
VOID FreeStructForFileContext(PFILE_OBJECT FileObject);

#endif //FILEFUNC_H
// conadlib.h: interface for the conadlib class.
//  CONSOL PROGRAMING ADVISOR LIBRARY
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONADLIB_H__09D047AC_4B33_4039_90CC_4655C7C20F5B__INCLUDED_)
#define AFX_CONADLIB_H__09D047AC_4B33_4039_90CC_4655C7C20F5B__INCLUDED_

#include<windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void gotoxy(int x, int y)
{
   COORD Pos = {x , y };

   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}


//*/
#endif // !defined(AFX_CONADLIB_H__09D047AC_4B33_4039_90CC_4655C7C20F5B__INCLUDED_)

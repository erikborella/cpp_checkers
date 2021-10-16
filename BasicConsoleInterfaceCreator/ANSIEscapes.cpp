#include "pch.h"
#include "ANSIEscapes.h"

void cursorUp(int n)
{
	printf(CUU, n);
}

void cursorDown(int n)
{
	printf(CUD, n);
}

void cursorForward(int n)
{
	printf(CUF, n);
}

void cursorBack(int n)
{
	printf(CUB, n);
}

void cursorNextLine(int n)
{
	printf(CNL, n);
}

void cursorPreviousLine(int n)
{
	printf(CPL, n);
}

void cursorHorizontalAbsolute(int n)
{
	printf(CHA, n);
}

void cursorPosition(int n, int m)
{
	printf(CUP, n, m);
}

void eraseDisplay(EDValuesEnum val)
{
	printf(ED, val);
}

void eraseLine(ELValuesEnum val)
{
	printf(EL, val);
}

void scroolUp(int n)
{
	printf(SU, n);
}

void scroolDown(int n)
{
	printf(SD, n);
}

void horizontalVerticalPosition(int n, int m)
{
	printf(HVP, n, m);
}

void selectGraphicRendition(const char* n)
{
	printf(SGR, n);
}

void deviceStatusReport(int* n, int* m)
{
	printf(DSR);

	while (_getch() != ESC[0]);
	while (_getch() != CSI[0]);

	char ch;
	int nTemp = 0;
	while ((ch = _getch()) != ';')
	{
		nTemp *= 10;
		nTemp += ch - '0';
	}

	int mTemp = 0;
	while ((ch = _getch()) != 'R')
	{
		mTemp *= 10;
		mTemp += ch - '0';
	}

	if (n != NULL)
		*n = nTemp;

	if (m != NULL)
		*m = mTemp;
}

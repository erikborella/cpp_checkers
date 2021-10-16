#include "pch.h"
#include "bcic.h"

#define returnInvalidIfNULL(ptr) if (ptr == NULL) return -1;

#define MENU_ARR_SIZE 100


struct bcic_s
{
	int nRows;
	int nCols;

	int originI;
	int originJ;

	bool usingHeader;
	bool usingFooter;

	int actionKeys;
};

struct _menu_s
{
	const char* desc;
	callback_f callback;
};

struct bcic_menu_s
{
	int ptr;
	_menu_s menus[MENU_ARR_SIZE];
};


bool _isArrowKey(unsigned char ch)
{
	return ch == 0 || ch == 224;
}

int _getConvertedActionKey(unsigned char ch)
{
	switch (ch)
	{
	case 13:
		return B_ACTION_ENTER;
	case 32:
		return B_ACTION_SPACE;
	default:
		return 0;
	}
}

bool _isActionKey(BCIC* bcic, unsigned char ch)
{
	return (_getConvertedActionKey(ch) & bcic->actionKeys) ? true:false;
}


void _bcic_markLine(BCIC* bcic, int line)
{
	cursorPosition(line, bcic->originJ);
	selectGraphicRendition("7");

	for (size_t i = 0; i < bcic->nCols; i++)
		putc(' ', stdout);

	selectGraphicRendition("0");
}

void _bcic_eraseLine(BCIC* bcic, int line)
{
	cursorPosition(line, bcic->originJ);
	selectGraphicRendition("0");

	for (size_t i = 0; i < bcic->nCols; i++)
		putc(' ', stdout);
}

void _bcic_createHeader(BCIC* bcic)
{
	_bcic_markLine(bcic, bcic->originI);
}

void _bcic_createFooter(BCIC* bcic)
{
	_bcic_markLine(bcic, bcic->originI + bcic->nRows);
}

int _bcic_calculateStartPrintingColumn(BCIC* bcic, const char* msg, HorizontalAligmentEnum aligment)
{
	switch (aligment)
	{
	case LEFT_ALIGN:
		return bcic->originJ;
	case CENTER_ALIGN:
		return ((bcic->nCols / 2) - (strlen(msg) / 2)) + bcic->originJ;
		break;
	case RIGHT_ALIGN:
		return (bcic->originJ + bcic->nCols) - (strlen(msg));
	default:
		return -1;
	}
}


void _bcic_printBox(BCIC* bcic, int originI, int originJ, int nRows, int nCols
/*, bool withShadow = false, int shadowLenght = 1*/)
{
	cursorPosition(originI, originJ);
	for (int i = 0; i < nCols; i++)
		fputc('-', stdout);

	for (int i = 1; i < nRows - 1; i++)
	{
		cursorPosition(originI + i, originJ);
		fputc('|', stdout);
		cursorForward(nCols - 2);
		fputc('|', stdout);
	}

	cursorPosition(originI + nRows - 1, originJ);
	for (int i = 0; i < nCols; i++)
		fputc('-', stdout);

	/*if (withShadow)
	{
		for (int i = 0; i < nRows; i++)
		{
			cursorPosition(originI + 1 + i, originJ + nCols);
			for (int j = 0; j < shadowLenght; j++)
			{
				putc('#', stdout);
			}
		}

		for (int i = 0; i < shadowLenght-1; i++)
		{
			cursorPosition(originI + nRows + i, originJ + 1);
			for (int j = 0; j < nCols+shadowLenght-1; j++)
			{
				putc('#', stdout);
			}
		}
	}*/
}


int _mod(int a, int b)
{
	int r = a % b;
	return r < 0 ? r + b : r;
}

BCIC* bcic_init(int nRows, int nCols, int originI, int originJ)
{
	BCIC* bcic = (BCIC*) malloc(sizeof(BCIC));

	if (bcic != NULL)
	{
		bcic->nRows = nRows;
		bcic->nCols = nCols;

		bcic->originI = originI;
		bcic->originJ = originJ;

		bcic->usingHeader = false;
		bcic->usingFooter = false;

		bcic->actionKeys = B_ACTION_ENTER | B_ACTION_SPACE;
	}

	return bcic;
}

void bcic_free(BCIC* bcic)
{
	free(bcic);
}


int bcic_printHeader(BCIC* bcic, const char* msg, HorizontalAligmentEnum aligment)
{
	returnInvalidIfNULL(bcic);

	if (!bcic->usingHeader)
	{
		_bcic_createHeader(bcic);
		bcic->usingHeader = true;
	}

	int startPrintingColumn = _bcic_calculateStartPrintingColumn(bcic, msg, aligment);

	selectGraphicRendition("7");

	cursorPosition(bcic->originI, startPrintingColumn);
	fputs(msg, stdout);

	selectGraphicRendition("0");

	return 1;
}

int bcic_clearHeader(BCIC* bcic)
{
	returnInvalidIfNULL(bcic);

	_bcic_createHeader(bcic);
	bcic->usingHeader = true;
}

int bcic_removeHeader(BCIC* bcic)
{
	returnInvalidIfNULL(bcic);

	_bcic_eraseLine(bcic, bcic->originI);
	bcic->usingHeader = false;

	return 1;
}



int bcic_printFooter(BCIC* bcic, const char* msg, HorizontalAligmentEnum aligment)
{
	returnInvalidIfNULL(bcic);

	if (!bcic->usingFooter)
	{
		_bcic_createFooter(bcic);
		bcic->usingFooter = true;
	}

	int startPrintingColumn = _bcic_calculateStartPrintingColumn(bcic, msg, aligment);

	selectGraphicRendition("7");

	cursorPosition(bcic->originI + bcic->nRows, startPrintingColumn);
	fputs(msg, stdout);

	selectGraphicRendition("0");

	cursorNextLine();

	return 1;
}

int bcic_clearFooter(BCIC* bcic)
{
	returnInvalidIfNULL(bcic);

	_bcic_createFooter(bcic);
	bcic->usingFooter = true;

	return 1;
}

int bcic_removeFooter(BCIC* bcic)
{
	returnInvalidIfNULL(bcic);

	_bcic_eraseLine(bcic, bcic->originI + bcic->nRows);
	bcic->usingFooter = false;

	return 1;
}

int bcic_setActionKeys(BCIC* bcic, int actionKeys)
{
	returnInvalidIfNULL(bcic);

	bcic->actionKeys = actionKeys;
	return 1;
}

int bcic_get2DCursorSelectionPosition(BCIC* bcic, 
	int originI, int originJ, 
	int maxRows, int maxCols, 
	int* retI, int* retJ,
	bool withBox)
{
	returnInvalidIfNULL(bcic);

	const int absOriginI = bcic->originI + originI - 1;
	const int absOriginJ = bcic->originJ + originJ - 1;

	const int absMaxI = absOriginI + maxRows;
	const int absMaxJ = absOriginJ + maxCols;

	int cursorI = absOriginI;
	int cursorJ = absOriginJ;

	if (withBox)
		_bcic_printBox(bcic, absOriginI-1, absOriginJ-1, maxRows + 2, maxCols + 2);


	while (true)
	{
		cursorPosition(cursorI, cursorJ);

		unsigned char ch = _getch();

		if (_isArrowKey(ch))
		{
			switch (_getch())
			{
			case KEY_UP:
				if (cursorI - 1 >= absOriginI)
					cursorI--;
				break;

			case KEY_DOWN:
				if (cursorI + 1 < absMaxI)
					cursorI++;
				break;

			case KEY_RIGHT:
				if (cursorJ + 1 < absMaxJ)
					cursorJ++;
				break;

			case KEY_LEFT:
				if (cursorJ - 1 >= absOriginJ)
					cursorJ--;
				break;
			}
		}
		else if (_getConvertedActionKey(ch) & bcic->actionKeys)
		{
			if (retI != NULL)
				*retI = cursorI - originI + 1;
			if (retJ != NULL)
				*retJ = cursorJ - originJ + 1;

			return 1;
		}
	}


}

int bcic_linearPrint(BCIC* bcic, const char* str, int originI, int originJ)
{
	returnInvalidIfNULL(bcic);

	const int absOriginI = bcic->originI + originI - 1;
	const int absOriginJ = bcic->originJ + originJ - 1;

	cursorPosition(absOriginI, absOriginJ);

	int i = 0;
	int lineCount = 0;

	while (str[i] != 0)
	{
		if (str[i] == '\n')
		{
			lineCount++;
			cursorPosition(absOriginI + lineCount, absOriginJ);
		}
		else
		{
			putc(str[i], stdout);
		}

		i++;
	}

	return lineCount;
}

int bcic_runMenu(BCIC* bcic, BCIC_MENU* menu, 
	int originI, int originJ)
{
	returnInvalidIfNULL(bcic);
	returnInvalidIfNULL(menu);

	int absOriginI;
	int absOriginJ;


	if (originI == -1)
		absOriginI = bcic->originI + bcic->usingHeader;
	else
		absOriginI = bcic->originI + originI - 1;

	if (originJ == -1)
		absOriginJ = bcic->originJ;
	else
		absOriginJ = bcic->originJ + originJ - 1;

	for (int i = 0; i < menu->ptr; i++)
	{
		cursorPosition(absOriginI + i, absOriginJ);
		fputs(menu->menus[i].desc, stdout);
	}

	int cursor = 0;

	while (true)
	{
		cursorPosition(absOriginI + cursor, absOriginJ);
		selectGraphicRendition("7");
		fputs(menu->menus[cursor].desc, stdout);
		selectGraphicRendition("0");
		cursorHorizontalAbsolute(absOriginJ);

		unsigned char ch = _getch();

		if (_isArrowKey(ch))
		{

			fputs(menu->menus[cursor].desc, stdout);

			switch (_getch())
			{
			case KEY_UP:
				cursor = _mod(cursor - 1, menu->ptr);
				break;

			case KEY_DOWN:
				cursor = _mod(cursor + 1, menu->ptr);
				break;
			}
		}
		else if (_isActionKey(bcic, ch))
		{
			if (menu->menus[cursor].callback != NULL)
				menu->menus[cursor].callback();

			return cursor;
		}
	}
}

void test(BCIC* bcic)
{
	int i = 5;

	while (true)
	{
		eraseDisplay();
		_bcic_printBox(bcic, 5, 5, 10, 10);
		_bcic_printBox(bcic, i, i, i, i);
		Sleep(200);
		i++;
	}
}



BCIC_MENU* bcic_menu_init()
{
	BCIC_MENU* menu = (BCIC_MENU*)malloc(sizeof(BCIC_MENU));

	if (menu != NULL)
	{
		menu->ptr = 0;
	}

	return menu;
}

void bcic_menu_free(BCIC_MENU* menu)
{
	free(menu);
}

int bcic_menu_add(BCIC_MENU* menu, const char* desc, callback_f callback)
{
	returnInvalidIfNULL(menu);

	if (menu->ptr >= MENU_ARR_SIZE)
		return 0;

	_menu_s newMenu
	{
		desc,
		callback
	};

	menu->menus[menu->ptr] = newMenu;
	menu->ptr++;

	return 1;
}

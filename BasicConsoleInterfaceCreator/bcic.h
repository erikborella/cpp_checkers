#pragma once

typedef struct bcic_s BCIC;
typedef struct bcic_menu_s BCIC_MENU;

typedef void (*callback_f) ();

#define B_ACTION_ENTER 1
#define B_ACTION_SPACE 1 << 1

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

enum HorizontalAligmentEnum
{
	LEFT_ALIGN,
	CENTER_ALIGN,
	RIGHT_ALIGN
};

BCIC* bcic_init(int nRows, int nCols, int originI = 1, int originJ = 1);
void bcic_free(BCIC* bcic);

int bcic_printHeader(BCIC* bcic, const char* msg, HorizontalAligmentEnum aligment);
int bcic_clearHeader(BCIC* bcic);
int bcic_removeHeader(BCIC* bcic);

int bcic_printFooter(BCIC* bcic, const char* msg, HorizontalAligmentEnum aligment);
int bcic_clearFooter(BCIC* bcic);
int bcic_removeFooter(BCIC* bcic);

int bcic_setActionKeys(BCIC* bcic, int actionKeys);

int bcic_get2DCursorSelectionPosition(BCIC* bcic, 
	int originI, int originJ, 
	int maxRows, int maxCols, 
	int* retI, int *retJ,
	bool withBox = false);

int bcic_linearPrint(BCIC* bcic, const char* str, int originI, int originJ);

int bcic_runMenu(BCIC* bcic, BCIC_MENU* menu, 
	int originI = -1, int originJ = -1);

void test(BCIC* bcic);





BCIC_MENU* bcic_menu_init();
void bcic_menu_free(BCIC_MENU* menu);

int bcic_menu_add(BCIC_MENU* menu, const char* desc, callback_f callback = NULL);

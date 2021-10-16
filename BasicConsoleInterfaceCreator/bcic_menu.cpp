#include "pch.h"
#include "bcic_menu.h"

#define returnInvalidIfNULL(ptr) if (ptr == NULL) return -1;

#define MENU_ARR_SIZE 100

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

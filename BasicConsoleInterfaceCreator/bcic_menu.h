
typedef struct bcic_menu_s BCIC_MENU;
typedef void (*callback_f) ();

BCIC_MENU* bcic_menu_init();
void bcic_menu_free(BCIC_MENU* menu);

int bcic_menu_add(BCIC_MENU* menu, const char* desc, callback_f callback = NULL);


/* Constructor: vtable, the two sub-objects at 4 and 0x18, flag at 0x24. */
typedef struct MenuA {
    void *vtable;
    char list[0x18 - 4];
    char cursor[0x24 - 0x18];
    unsigned char open;
} MenuA;

extern char D_089CCAD0[];
extern void func_088EC4C4(void *list);
extern void func_088EC278(void *cursor);

MenuA *func_088EC6E8(MenuA *menu)
{
    menu->vtable = D_089CCAD0;
    func_088EC4C4(menu->list);
    func_088EC278(menu->cursor);
    menu->open = 0;
    return menu;
}

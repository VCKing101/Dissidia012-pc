typedef struct MenuB {
    void *vtable;
    char list[0x18 - 4];
    char cursor[0x24 - 0x18];
    int field_24;
    int field_28;
    int field_2C;
    unsigned char open;
} MenuB;

extern char D_089CCB08[];
extern void func_088EC4C4(void *list);
extern void func_088EC278(void *cursor);

MenuB *func_088EC944(MenuB *menu)
{
    menu->vtable = D_089CCB08;
    func_088EC4C4(menu->list);
    func_088EC278(menu->cursor);
    menu->field_24 = 0;
    menu->field_28 = 0;
    menu->field_2C = 0;
    menu->open = 0;
    return menu;
}

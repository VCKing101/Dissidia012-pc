typedef struct MenuList {
    char pad_000[0x6AC];
    char items_6AC[1];
} MenuList;

typedef struct MenuCursor {
    char pad_00[0x24];
    MenuList *list;
    char pad_28[0x34];
    int state;
    char pad_60[0xC];
    int fallback;
    int pad_70;
    int selected;
    int field_78;
} MenuCursor;

extern int func_08AF5AC0(void *items);
extern void func_08AF605C(MenuList *list, int index);

void func_08AF3938(MenuCursor *self)
{
    if (self->list != 0) {
        int index = func_08AF5AC0(self->list->items_6AC);
        self->selected = index != -1 ? index : self->fallback;
        func_08AF605C(self->list, self->selected);
    }
    self->field_78 = 0;
    self->state = 6;
}

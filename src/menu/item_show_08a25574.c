typedef struct MenuItemRef {
    int field_00;
    int field_04;
} MenuItemRef;

extern MenuItemRef func_088B97E8(unsigned short id);
extern void func_088C1998(MenuItemRef *ref);

void func_08A25574(void *self, unsigned short id) {
    MenuItemRef ref = func_088B97E8(id);
    func_088C1998(&ref);
}

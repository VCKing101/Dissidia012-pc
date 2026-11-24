/* Handles event ids: 0xC sets state 1, 0xB advances to 7, 0xA closes the sub-objects and either returns to 0 or sets state 5; 8 does nothing. */
extern void func_08A77F60(int self, int a);
extern void func_08A743E0(int p);
extern void func_08A72F5C(int p);
extern void func_088F69F8(void);
void func_08A77D1C(int self, int unused, int ev) {
    switch (ev) {
    case 8:
        break;
    case 10:
        func_08A743E0(self + 4);
        func_08A72F5C(self + 0x14);
        if (*(int *)(self + 0x30) == 4) {
            func_088F69F8();
            func_08A77F60(self, 0);
        } else {
            *(int *)(self + 0x30) = 5;
        }
        break;
    case 12:
        *(int *)(self + 0x30) = 1;
        break;
    case 11:
        func_08A77F60(self, 7);
        break;
    }
}

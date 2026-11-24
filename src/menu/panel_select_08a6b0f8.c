/* Activates the object at the table-selected slot (mode 3), runs two follow-ups and sets the state word to 1. */
extern unsigned short D_08A6B358[];
extern void func_088D9F5C(int obj, int a, unsigned char b);
extern void func_08A80F14(int obj, int a);
extern void func_08A6A87C(int self);
void func_08A6B0F8(int self) {
    unsigned short slot = D_08A6B358[*(int *)(self + 0x674) * 5 + 1];
    func_088D9F5C(*(int *)(self + slot * 4 + 0x5B8), 3, 1);
    func_08A80F14(*(int *)(self + 0x434), 0);
    func_08A6A87C(self);
    *(int *)(self + 0x28) = 1;
}

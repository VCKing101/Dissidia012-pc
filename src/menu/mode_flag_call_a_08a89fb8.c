/* Calls the manager routine when the mode byte is not 1 and bit 0x2000 of the owner's flags is set. */
extern unsigned char D_089D5F20;
extern int D_089D7F90;
extern void func_088FCB8C(int v);
void func_08A89FB8(int self) {
    if (D_089D5F20 == 1) {
        return;
    }
    if ((*(unsigned *)(*(int *)(self + 0x30C) + 0x41C) & 0x2000) == 0) {
        return;
    }
    func_088FCB8C(D_089D7F90);
}

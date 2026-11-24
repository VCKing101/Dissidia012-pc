/* Calls the manager routine unless the mode byte is 1. */
extern unsigned char D_089D5F20;
extern int D_089D7F90;
extern void func_088FCA30(int v);
void func_08A89F40(void) {
    if (D_089D5F20 == 1) {
        return;
    } else {
        func_088FCA30(D_089D7F90);
    }
}

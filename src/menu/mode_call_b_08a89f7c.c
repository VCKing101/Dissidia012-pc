/* Returns 1 when the mode byte is 1, otherwise the manager routine's result. */
extern unsigned char D_089D5F20;
extern int D_089D7F90;
extern int func_088FCAE8(int v);
int func_08A89F7C(void) {
    int one = 1;
    if (D_089D5F20 == one) {
        return 1;
    } else {
        return func_088FCAE8(D_089D7F90);
    }
}

/* Starts one of three effects depending on the mode byte at +0x4F (0, 1, 2). The volatile read keeps retail's sw ra before the lbu. */
extern void func_089A12A4(int, int, int, int, int);
void func_0895DE24(int a, void *s) {
    switch (*((volatile unsigned char *)s + 0x4F)) {
    case 0:
        func_089A12A4(0, 6, 7, 0, 0);
        break;
    case 1:
        func_089A12A4(0, 6, 10, 0, -1);
        break;
    case 2:
        func_089A12A4(2, 6, 10, 0, -1);
        break;
    }
}

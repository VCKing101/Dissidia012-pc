/* If D_089D5F20 is clear, returns func_088FCAE8(D_089D7F90); otherwise returns 1. */
extern unsigned char D_089D5F20;
extern int D_089D7F90;
extern int func_088FCAE8(int a);
int func_08AF7504(void) {
    int r;
    switch (D_089D5F20) {
    default:
        r = 1;
        break;
    case 0:
        r = func_088FCAE8(D_089D7F90);
        break;
    }
    return r;
}

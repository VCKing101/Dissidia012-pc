/* If D_089D5F20 is clear, returns func_088FDC98(D_089D7F90); otherwise returns 0. */
extern unsigned char D_089D5F20;
extern int D_089D7F90;
extern int func_088FDC98(int a);
int func_08AF7494(void) {
    switch (D_089D5F20) {
    default:
        return 0;
    case 0:
        return func_088FDC98(D_089D7F90);
    }
}

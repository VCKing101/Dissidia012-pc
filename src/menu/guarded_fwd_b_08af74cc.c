/* If D_089D5F20 is clear, forwards D_089D7F90 to func_088FCAB8. */
extern unsigned char D_089D5F20;
extern int D_089D7F90;
extern void func_088FCAB8(int a);
void func_08AF74CC(void) {
    switch (D_089D5F20) {
    default:
        break;
    case 0:
        func_088FCAB8(D_089D7F90);
        break;
    }
}

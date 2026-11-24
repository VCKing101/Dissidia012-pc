/* Returns whether func_088F0C80() equals func_089C5D70 of the three halfwords at a1. */
extern int func_089C5D70(int a, int b, int c);
extern int func_088F0C80(void);
int func_08AC4930(int a0, unsigned short *p) {
    int x = func_089C5D70(p[0], p[1], p[2]);
    return func_088F0C80() == x;
}

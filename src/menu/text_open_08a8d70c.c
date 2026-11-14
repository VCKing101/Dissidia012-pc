extern char D_08A939D0[];
extern char D_08A93AF2[];
extern char D_08A93B06[];
extern void func_08A8D55C(void *self, int target, int lines, int a, int b, char *text, unsigned char flag);

void func_08A8D70C(void *self, int kind, int target, unsigned char flag)
{
    if (kind == 0) {
        func_08A8D55C(self, target, 3, 1, 5, D_08A939D0, flag);
    } else if (kind == 1) {
        func_08A8D55C(self, target, 3, 0, 0x66, D_08A93AF2, flag);
    } else if (kind == 2) {
        func_08A8D55C(self, target, 2, 1, 0x81, D_08A93B06, flag);
    }
}

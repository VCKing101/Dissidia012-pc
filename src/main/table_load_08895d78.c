extern unsigned char D_089CA920[];
void func_08895DA0(void);
void func_088823C4(int a, int b, int c, void (*callback)(void), void *table);

void func_08895D78(int a, int b, int c) {
    func_088823C4(a, b, c, func_08895DA0, D_089CA920);
}

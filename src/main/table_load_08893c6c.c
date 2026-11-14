extern unsigned char D_089CA804[];
void func_08893218(void);
void func_088823C4(int a, int b, int c, void (*callback)(void), void *table);

void func_08893C6C(int a, int b, int c) {
    func_088823C4(a, b, c, func_08893218, D_089CA804);
}

extern int D_089C6C7C;
void func_08807538(void);
void func_088823C4(int a, int b, int c, void (*callback)(void), void *table);

void func_08808424(int a, int b, int c) {
    func_088823C4(a, b, c, func_08807538, &D_089C6C7C);
}

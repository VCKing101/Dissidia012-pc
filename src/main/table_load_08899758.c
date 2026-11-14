extern unsigned char D_089CAB18[];
void func_08899380(void);
void func_088823C4(int a, int b, int c, void (*callback)(void), void *table);

void func_08899758(int a, int b, int c) {
    func_088823C4(a, b, c, func_08899380, D_089CAB18);
}

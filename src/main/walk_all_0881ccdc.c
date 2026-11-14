typedef struct Agent4Walker {
    unsigned char unknown_0000[8];
    unsigned char done;
} Agent4Walker;

void *func_0881CB80(void *previous);
void func_088191F8(void *item);
Agent4Walker *func_0881BAA8(void);

void func_0881CCDC(void) {
    void *item = 0;
    while ((item = func_0881CB80(item)) != 0) {
        func_088191F8(item);
    }
    func_0881BAA8()->done = 1;
}

typedef struct Slot08899688 {
    unsigned char data[0x108];
} Slot08899688;

typedef struct Bank08899688 {
    Slot08899688 main;
    Slot08899688 extra[1];
    int index;
} Bank08899688;

Bank08899688 *func_088992B0(void);
void func_08898EE8(Slot08899688 *slot);

void func_08899688(void) {
    Bank08899688 *bank = func_088992B0();
    int i;
    func_08898EE8(&bank->main);
    for (i = 0; i < 1; i++) {
        func_08898EE8(&bank->extra[i]);
    }
    bank->index = -1;
}

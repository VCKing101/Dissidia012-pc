typedef struct Slot08899300 {
    unsigned char data[0x108];
} Slot08899300;

typedef struct Bank08899300 {
    Slot08899300 slots[2];
    int index;
} Bank08899300;

Bank08899300 *func_088992B0(void);

Slot08899300 *func_08899300(void) {
    Bank08899300 *bank = func_088992B0();
    int front = bank->index == 0;
    if (front) {
        return &bank->slots[bank->index + 1];
    }
    return bank->slots;
}

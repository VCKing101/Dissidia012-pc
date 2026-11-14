typedef struct Slot088996F0 {
    unsigned char data[0x108];
} Slot088996F0;

typedef struct Bank088996F0 {
    Slot088996F0 main;
    Slot088996F0 extra[1];
    int index;
} Bank088996F0;

Bank088996F0 *func_088992B0(void);
void func_08898F5C(Slot088996F0 *slot);

void func_088996F0(void) {
    Bank088996F0 *bank = func_088992B0();
    int i;
    func_08898F5C(&bank->main);
    for (i = 0; i < 1; i++) {
        func_08898F5C(&bank->extra[i]);
    }
    bank->index = -1;
}

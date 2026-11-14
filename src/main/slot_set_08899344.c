typedef struct Slot08899344 {
    unsigned char unknown_000[0xFC];
    int value;
    int mode;
} Slot08899344;

Slot08899344 *func_08899300(void);

void func_08899344(int value) {
    int mode = 4;
    func_08899300()->mode = mode;
    func_08899300()->value = value;
}

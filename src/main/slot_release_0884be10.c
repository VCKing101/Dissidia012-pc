typedef struct Slot0884BE10 {
    unsigned char id;
    unsigned char unknown_01[3];
    void *field_04;
    void *field_08;
    int field_0C;
    int field_10;
} Slot0884BE10;

typedef struct Table0884BE10 {
    unsigned char unknown_0000[0x258C];
    Slot0884BE10 slots[32];
} Table0884BE10;

extern int D_089D4D80;
void func_0884C35C(int heap);
void func_0899BB1C(void *p);

void func_0884BE10(Table0884BE10 *self) {
    int i;
    func_0884C35C(D_089D4D80);
    for (i = 1; i < 32; i++) {
        func_0899BB1C(self->slots[i].field_04);
        self->slots[i].field_04 = 0;
        func_0899BB1C(self->slots[i].field_08);
        self->slots[i].field_08 = 0;
    }
}

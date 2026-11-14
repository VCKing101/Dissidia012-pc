typedef struct Slot0884BDD4 {
    unsigned char id;
    unsigned char unknown_01[3];
    void *field_04;
    void *field_08;
    int field_0C;
    int field_10;
} Slot0884BDD4;

typedef struct Table0884BDD4 {
    unsigned char unknown_0000[0x2580];
    int field_2580;
    int field_2584;
    int field_2588;
    Slot0884BDD4 slots[32];
} Table0884BDD4;

void func_0884BDD4(Table0884BDD4 *self) {
    int i;
    self->field_2588 = 0;
    self->field_2584 = 0;
    self->field_2580 = 0;
    for (i = 1; i < 32; i++) {
        Slot0884BDD4 *slot = &self->slots[i];
        slot->field_10 = 0;
        slot->field_0C = 0;
        slot->id = i;
    }
}

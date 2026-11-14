typedef struct Item08854A18 {
    unsigned char unknown_00[0x44];
    float value;
    unsigned char unknown_48[0x18];
} Item08854A18;

typedef struct Owner08854A18 {
    int field_00;
    int field_04;
    Item08854A18 *items;
} Owner08854A18;

extern "C" void func_08854A18(Owner08854A18 *self) {
    if (self->items == 0) {
        return;
    }
    for (int i = 0; i < 40; i++) {
        self->items[i].value = 0.0f;
    }
}

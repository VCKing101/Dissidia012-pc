typedef struct CursorState {
    char pad_0000[0xDD0];
    unsigned short field_DD0[0x244];
    int field_1258;
    int field_125C;
    int field_1260;
} CursorState;

typedef struct CursorOwner {
    char pad_0000[0x1274];
    CursorState *field_1274;
} CursorOwner;

void func_08A39078(CursorOwner *self, int id)
{
    int i;
    self->field_1274->field_125C = id;
    self->field_1274->field_1260 = self->field_1274->field_125C;
    for (i = 0; i < self->field_1274->field_1258; i++) {
        if (self->field_1274->field_DD0[i] == id) {
            self->field_1274->field_1260 = i;
        }
    }
}

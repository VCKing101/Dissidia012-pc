typedef struct A6Info {
    char pad_00[8];
    unsigned char field_08;
    char pad_09;
    char field_0A;
} A6Info;

typedef struct A6Tile {
    char pad_000[0x40C];
    int field_40C;
    char pad_410[0x15C];
    A6Info *field_56C;
} A6Tile;

void func_08A81740(int obj, int kind, int value);
void func_08A80F14(int obj, int one);
void func_08A8161C(int obj, int one, int zero);

void func_08A5BB5C(A6Tile *self) {
    if (self->field_40C != 0) {
        func_08A81740(self->field_40C, self->field_56C->field_08, self->field_56C->field_0A);
        func_08A80F14(self->field_40C, 1);
        if (self->field_56C->field_08 == 0x17) {
            func_08A8161C(self->field_40C, 1, 0);
        }
    }
}

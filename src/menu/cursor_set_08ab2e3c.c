typedef struct CursorRect {
    int field_00;
    unsigned char field_04;
    char pad_05;
    unsigned short field_06;
    int field_08;
    int field_0C;
} CursorRect;

typedef struct CursorState {
    int field_00;
    int field_04;
    unsigned short field_08;
    unsigned char field_0A;
    char pad_0B;
    int field_0C;
    char pad_010[0x56C - 0x10];
    CursorRect field_56C;
    int field_57C;
} CursorState;

typedef struct CursorOwner {
    char pad_000[0x580];
    int field_580;
    CursorState *field_584;
} CursorOwner;

void func_08AB2E3C(CursorOwner *self, int kind, CursorRect *rect, int value)
{
    self->field_580 = kind;
    self->field_584->field_04 = 1;
    self->field_584->field_56C = *rect;
    self->field_584->field_0C = self->field_584->field_56C.field_00;
    self->field_584->field_0A = self->field_584->field_56C.field_04;
    self->field_584->field_08 = self->field_584->field_56C.field_06;
    self->field_584->field_57C = value;
}

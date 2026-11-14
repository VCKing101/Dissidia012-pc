/* Moves the hint window to (x - 20, y - 6), clamped to 0..limit on each axis. */
typedef struct HintBox {
    char pad_000[0x112];
    short field_112;
    short field_114;
    short field_116;
    short field_118;
} HintBox;

typedef struct HintState {
    char pad_0000[0x38A0];
    HintBox field_38A0;
} HintState;

typedef struct HintOwner {
    char pad_000[0x14C];
    HintState *field_14C;
} HintOwner;

extern void func_089FE180(HintOwner *self);

void func_089FD2E8(HintOwner *self, short x, short y)
{
    HintBox *box = &self->field_14C->field_38A0;
    box->field_112 = x - 20;
    if (box->field_112 < 0) {
        box->field_112 = 0;
    } else if (box->field_116 < box->field_112) {
        box->field_112 = box->field_116;
    }
    box->field_114 = y - 6;
    if (box->field_114 < 0) {
        box->field_114 = 0;
    } else if (box->field_118 < box->field_114) {
        box->field_114 = box->field_118;
    }
    func_089FE180(self);
}

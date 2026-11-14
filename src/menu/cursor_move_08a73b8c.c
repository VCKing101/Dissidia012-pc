/* Moves the highlight from item `from` to item `to` (negative: none). */
typedef struct MoveItem {
    char data[0x18];
} MoveItem;

typedef struct MovePanel {
    char pad_000[0x2C];
    MoveItem field_2C[(0x14C - 0x2C) / 0x18];
    int field_14C;
} MovePanel;

extern void func_08A7345C(MoveItem *item, int on);
extern void func_088D319C(MovePanel *self);
extern void func_088D3174(MovePanel *self);

void func_08A73B8C(MovePanel *self, int to, int from)
{
    if (from >= 0) {
        func_08A7345C(&self->field_2C[from], 0);
    } else {
        func_088D319C(self);
    }
    if (to >= 0) {
        func_08A7345C(&self->field_2C[to], 1);
    } else if (self->field_14C == 1) {
        func_088D3174(self);
    }
}

/* Resets the scroller for the current mode (0 locks it on the first row). */
typedef struct ResetScroller {
    unsigned int field_00;
} ResetScroller;

typedef struct ResetScrollPanel {
    char pad_00[0x28];
    int field_28;
    char pad_2C[0xA4 - 0x2C];
    ResetScroller field_A4;
} ResetScrollPanel;

extern void func_088D46E4(ResetScroller *scroller, int first, int zero, int last, int none, int mode);

void func_08A68CD8(ResetScrollPanel *self)
{
    switch (self->field_28) {
    case 0:
        func_088D46E4(&self->field_A4, 1, 0, 1, 0, -1);
        self->field_A4.field_00 = self->field_A4.field_00 | 1;
        break;
    case 1:
    case 2:
        func_088D46E4(&self->field_A4, 0, 0, 1, 0, -1);
        break;
    }
}

/* Switches the view to match the grid's current layout (layouts 0-3). */
typedef struct LayoutState {
    char pad_0000[0x6B1C];
    void *field_6B1C;
    char pad_6B20[0x6B48 - 0x6B20];
    void *field_6B48;
} LayoutState;

typedef struct LayoutOwner {
    char pad_00[0x58];
    LayoutState *field_58;
} LayoutOwner;

extern int func_08A6A00C(void *grid);
extern void func_08A697B0(void *view, int mode, int arg);

void func_08A515F0(LayoutOwner *self)
{
    switch (func_08A6A00C(self->field_58->field_6B48)) {
    case 0:
    case 3:
        func_08A697B0(self->field_58->field_6B1C, 0, -1);
        break;
    case 1:
        func_08A697B0(self->field_58->field_6B1C, 5, 4);
        break;
    case 2:
        func_08A697B0(self->field_58->field_6B1C, 4, 4);
        break;
    }
}

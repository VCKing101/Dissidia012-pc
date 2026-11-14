/* Applies the panel's mode: 1 rebuilds and redraws the list, 2 refreshes and redraws it. */
typedef struct ApplyPanel {
    char pad_00[0x28];
    int field_28;
} ApplyPanel;

extern void func_08A6A27C(ApplyPanel *self);
extern void func_08A6A260(ApplyPanel *self);
extern void func_08A6A20C(ApplyPanel *self);
extern void func_08A6A428(ApplyPanel *self);

void func_08A6A184(ApplyPanel *self)
{
    switch (self->field_28) {
    case 1:
        func_08A6A27C(self);
        func_08A6A260(self);
        func_08A6A20C(self);
        break;
    case 2:
        func_08A6A428(self);
        func_08A6A20C(self);
        break;
    case 3:
        break;
    }
}

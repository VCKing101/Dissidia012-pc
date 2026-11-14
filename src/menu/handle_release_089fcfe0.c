typedef struct HandleState {
    char pad_00000[0x2A608];
    int field_2A608;
} HandleState;

typedef struct HandleOwner {
    char pad_000[0x14C];
    HandleState *field_14C;
} HandleOwner;

extern void func_088DAB44(int handle);

void func_089FCFE0(HandleOwner *self)
{
    if (self->field_14C->field_2A608 != 0) {
        func_088DAB44(self->field_14C->field_2A608);
        self->field_14C->field_2A608 = 0;
    }
}

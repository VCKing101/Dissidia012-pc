typedef struct PickState {
    char pad_0000[0x47C];
    int field_47C;
    char pad_0480[0x1B7C - 0x480];
    unsigned int field_1B7C;
} PickState;

typedef struct PickOwner {
    char pad_0000[0x46AC];
    PickState *field_46AC;
} PickOwner;

extern void func_088D9CFC(int handle);
extern void func_088D9F5C(int target, int kind, int value);
extern void func_08ADA984(PickOwner *self);
extern void func_08ADCAB8(PickOwner *self);

void func_08ADA924(PickOwner *self)
{
    if ((self->field_46AC->field_1B7C & 2) != 0) {
        func_088D9CFC(self->field_46AC->field_47C);
    } else {
        func_088D9F5C(self->field_46AC->field_47C, 0x1D, 1);
    }
    func_08ADA984(self);
    func_08ADCAB8(self);
}

typedef struct BoardState {
    char pad_0000[0x47C];
    int field_47C;
    char pad_0480[0x2558 - 0x480];
    unsigned int field_2558;
} BoardState;

typedef struct BoardOwner {
    char pad_00000[0xA690];
    BoardState *field_A690;
} BoardOwner;

extern void func_088D9CFC(int handle);
extern void func_088D9F5C(int target, int kind, int value);
extern void func_08AE4D28(BoardOwner *self);
extern void func_08AE7A8C(BoardOwner *self);

void func_08AE4CC0(BoardOwner *self)
{
    if ((self->field_A690->field_2558 & 2) != 0) {
        func_088D9CFC(self->field_A690->field_47C);
    } else {
        func_088D9F5C(self->field_A690->field_47C, 3, 1);
    }
    func_08AE4D28(self);
    func_08AE7A8C(self);
}

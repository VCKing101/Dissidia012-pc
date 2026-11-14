/* Acts on the dialog answer: 1 fades out to state 3, 2 fades out and restarts to state 9. */
typedef struct ResultData {
    char pad_0000[0x4];
    int field_04;
    char pad_0008[0x6B20 - 0x8];
    void *field_6B20;
} ResultData;

typedef struct ResultOwner {
    char pad_00[0x4];
    int field_04;
    char pad_08[0x48 - 0x8];
    char field_48[0x10];
    ResultData *field_58;
} ResultOwner;

extern int func_08A60A2C(void *dialog);
extern void func_08A50740(ResultOwner *self);
extern void func_088D54DC(void *timer);

void func_08A5095C(ResultOwner *self)
{
    switch (func_08A60A2C(self->field_58->field_6B20)) {
    case 0:
        break;
    case 1:
        func_08A50740(self);
        self->field_58->field_04 = 3;
        break;
    case 2:
        func_08A50740(self);
        func_088D54DC(self->field_48);
        self->field_04 = 0;
        self->field_58->field_04 = 9;
        break;
    }
}

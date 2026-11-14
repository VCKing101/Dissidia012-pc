typedef struct InitRec {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
    char field_08;
    char field_09;
    char pad_0A[2];
    char field_0C;
    char field_0D;
    char pad_0E[2];
    char field_10[1];
} InitRec;

typedef struct InitOwner {
    char pad_00[8];
    InitRec **field_08;
} InitOwner;

extern void func_0892A13C(void *target);

int func_08A5C0D0(InitOwner *self)
{
    InitRec *rec = *self->field_08;
    rec->field_00 = -1;
    rec->field_02 = 0;
    rec->field_04 = 0;
    rec->field_06 = 0;
    rec->field_08 = -1;
    rec->field_09 = -1;
    rec->field_0C = -1;
    rec->field_0D = -1;
    func_0892A13C(rec->field_10);
    return 1;
}

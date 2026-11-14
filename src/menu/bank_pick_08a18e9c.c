typedef struct BankInner {
    char pad_000[0x400];
    int field_400;
    char pad_404[0x880 - 0x404];
    int field_880[1];
    char pad_884[0x10F8 - 0x884];
    int field_10F8;
    char pad_10FC[0x112C - 0x10FC];
    unsigned char field_112C;
} BankInner;

typedef struct BankOwner {
    char pad_0000[0x17B0];
    BankInner *field_17B0;
} BankOwner;

extern void func_0887B828(int handle, void *arg);
extern void func_088D44EC(void *a, void *b, unsigned short *table);
extern unsigned short D_08A1AC28;
extern unsigned short D_08A1AC60;
extern unsigned short D_08A1B0EE;
extern int D_08A1B108;

void func_08A18E9C(BankOwner *self)
{
    BankInner *inner = self->field_17B0;
    unsigned char flag;
    if (inner->field_10F8 == 0) {
        func_088D44EC(&inner->field_400, inner->field_880, &D_08A1AC28);
        inner = self->field_17B0;
        flag = inner->field_112C;
    } else {
        func_088D44EC(&inner->field_400, inner->field_880, &D_08A1AC60);
        inner = self->field_17B0;
        flag = inner->field_112C;
    }
    if (flag != 0) {
        func_0887B828(inner->field_880[D_08A1B0EE], &D_08A1B108);
    }
}

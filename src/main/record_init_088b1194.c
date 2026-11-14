typedef struct Record { char data_000[0x34]; int field_34; int owner; char data_03C[0x23C - 0x3C]; int extra; char data_240[0x588 - 0x240]; int field_588; int field_58C; } Record;
extern unsigned char *D_089D7280;
extern void func_08935828(void *dst, int value, int size);
extern void func_088B168C(unsigned char *src, Record *dst, int size);
extern int func_088B1054(Record *r);

int func_088B1194(Record *r, int owner, int a, int b, int extra)
{
    if (*D_089D7280 != 0) {
        return 0;
    }
    r->field_588 = a;
    r->field_58C = b;
    func_08935828(r, 0, 0x2C4);
    func_088B168C(D_089D7280, r, 0x2C4);
    r->field_34 = 0;
    r->owner = owner;
    r->extra = extra;
    return func_088B1054(r);
}

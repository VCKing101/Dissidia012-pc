/* Stores a value into field +0xC of record i (stride 0x2E8, base +0xE94) of self. */
typedef struct Rec { char pad[8]; int f8; int fc; char rest[0x2E8 - 16]; } Rec;
void func_08B42F1C(char *self, int i, int v)
{
    Rec *r = (Rec *)(self + i * 0x2E8 + 0xE94);
    r->fc = v;
}

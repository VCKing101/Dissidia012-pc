struct ResetInner {
    char pad_000[0x400];
    int field_400;
    char pad_404[0x5C0 - 0x404];
    int field_5C0;
    char pad_5C4[0x9EC - 0x5C4];
    int field_9EC[7];
    char pad_A08[0x1010 - 0xA08];
    unsigned int field_1010;
};

struct ResetOwner {
    char pad_0000[0x1320];
    ResetInner *field_1320;
};

extern "C" {

void func_0887B944(int handle);
void func_088D3EA0(ResetInner *inner, void *table_target, unsigned short *table);
void func_088D9CFC(int handle);
extern unsigned short D_08A11B7E;

void func_08A0E070(ResetOwner *self)
{
    self->field_1320->field_1010 = self->field_1320->field_1010 & ~2;
    self->field_1320->field_1010 = self->field_1320->field_1010 & ~4;
    func_088D3EA0(self->field_1320, &self->field_1320->field_400, &D_08A11B7E);
    func_088D9CFC(self->field_1320->field_5C0);
    for (int i = 0; i < 7; i++) {
        func_0887B944(self->field_1320->field_9EC[i]);
    }
}

}

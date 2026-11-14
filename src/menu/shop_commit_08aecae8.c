typedef struct ShopSlot {
    signed char field_00;
    char pad_01[0xB];
} ShopSlot;

typedef struct ShopState {
    char pad_0000[0x400];
    char field_400[0x1924 - 0x400];
    unsigned int field_1924;
    char pad_1928[0x195C - 0x1928];
    char field_195C[4];
    int field_1960;
    char pad_1964b[0x19B8 - 0x1964];
    int field_19B8;
    int field_19BC;
    int pad_19C0;
    unsigned short field_19C4[0x12];
    ShopSlot field_19E8[0x3E];
    char pad_1CD0[0x1CE0 - 0x1CD0];
    unsigned short field_1CE0[3][5];
    char pad_1CFE[0x1D12 - 0x1CFE];
    unsigned char field_1D12[0x40];
} ShopState;

typedef struct ShopOwner {
    char pad_0000[0x23F0];
    ShopState *field_23F0;
} ShopOwner;

extern int D_08B01CF8;
extern void func_08AF6E10(int target, int kind);
extern void func_08AEC700(ShopOwner *self, int row);
extern void func_08AECD10(ShopOwner *self);
extern void func_08AECDC4(ShopOwner *self);
extern void func_088D4788(char *list, int *ids, int count);
extern void func_088D4928(char *list, int value);
extern void func_08AEDBE8(ShopOwner *self);
extern void func_08AEC0C8(ShopOwner *self);
extern void func_08AEBE58(ShopOwner *self);

void func_08AECAE8(ShopOwner *self)
{
    int i;
    self->field_23F0->field_1924 = self->field_23F0->field_1924 & ~0x2004;
    func_08AF6E10(D_08B01CF8, 0x18);
    for (i = 0; i < 5; i++) {
        self->field_23F0->field_1CE0[self->field_23F0->field_19B8][i] = self->field_23F0->field_19C4[i];
    }
    func_08AEC700(self, self->field_23F0->field_19B8);
    func_08AECD10(self);
    func_08AECDC4(self);
    func_088D4788(self->field_23F0->field_195C, 0, 0);
    func_088D4928(self->field_23F0->field_195C, 0);
    func_08AEDBE8(self);
    func_08AEC0C8(self);
    func_08AEBE58(self);
}

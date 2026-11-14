typedef struct MenuRootState {
    char pad_0000[0x2BB8];
    unsigned int field_2BB8;
} MenuRootState;

typedef struct MenuRootOwner {
    char pad_000[0x154];
    MenuRootState *field_154;
} MenuRootOwner;

typedef struct MenuNoticePair {
    unsigned short field_00;
    unsigned short field_02;
} MenuNoticePair;

extern unsigned short D_08A23AA4[];
extern unsigned short D_08A23ABA[];
extern unsigned short D_08A23AC6[];
extern unsigned short D_08A23ACA[];
extern MenuNoticePair D_08A23B0C[];

extern void func_08A1C2F8(MenuRootOwner *self);
extern void func_08A1D42C(MenuRootOwner *self, int a, unsigned short text, int b, int c);
extern void func_08A1D460(MenuRootOwner *self, int a, int b, unsigned short text, int c, int d);

void func_08A1D6E4(MenuRootOwner *self) {
    self->field_154->field_2BB8 |= 2;
    func_08A1C2F8(self);
}

void func_08A1D70C(MenuRootOwner *self) {
    self->field_154->field_2BB8 &= ~2;
    func_08A1C2F8(self);
}

void func_08A1D738(MenuRootOwner *self, short index) {
    func_08A1D42C(self, 0, D_08A23AA4[index], 0, 0);
}

void func_08A1D778(MenuRootOwner *self, short index) {
    func_08A1D42C(self, 0, D_08A23ABA[index], 0, 0);
}

void func_08A1D7B8(MenuRootOwner *self, short index) {
    func_08A1D42C(self, 0, D_08A23AC6[index], 0, 0);
}

void func_08A1D7F8(MenuRootOwner *self, short index) {
    func_08A1D460(self, 3, 0, D_08A23ACA[index], 0, 0);
}

void func_08A1D83C(MenuRootOwner *self, short index) {
    func_08A1D42C(self, 0, D_08A23B0C[index].field_00, 0, 0);
}

unsigned short func_08A1D87C(MenuRootOwner *self, short index) {
    return D_08A23B0C[index].field_02;
}

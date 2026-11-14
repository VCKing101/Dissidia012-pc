typedef struct MenuHelpPair {
    int field_00;
    int field_04;
} MenuHelpPair;

typedef struct MenuHelpState {
    char pad_00[0xC];
    int field_0C[1];
    int field_10[0x17];
    int field_6C;
} MenuHelpState;

typedef struct MenuHelpOwner {
    char pad_000[0xF64];
    MenuHelpState *field_F64;
} MenuHelpOwner;

extern MenuHelpPair D_08A38FBC[][3];
extern void func_08A37180(MenuHelpOwner *self);
extern void func_0887B924(int handle);

void func_08A371E4(MenuHelpOwner *self) {
    MenuHelpPair *pair;

    func_08A37180(self);
    pair = D_08A38FBC[self->field_F64->field_6C];
    while (pair->field_00 != -1) {
        int id = pair->field_00;
        func_0887B924(self->field_F64->field_0C[id]);
        func_0887B924(self->field_F64->field_10[id]);
        pair++;
    }
}

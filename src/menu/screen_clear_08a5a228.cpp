struct A6Row {
    char data[0x68];
};

struct A6Cell {
    char data[0x4C4];
};

struct A6Screen3 {
    char pad_00[0x44];
    char field_44[0x10];
    char field_54[0x24];
    A6Row field_78[3];
    A6Cell field_1B0[5];
};

extern "C" {
void func_088D3F00(int a, char *b, void (*cb)(void));
void func_088D44EC(char *a, char *b, void (*cb)(void));
void func_08A5B6D0(void);
void func_08A5B6F8(void);
void func_08A589CC(A6Row *r);
void func_08A5919C(A6Cell *c);

void func_08A5A228(A6Screen3 *self) {
    func_088D3F00(0, self->field_44, func_08A5B6D0);
    func_088D44EC(self->field_44, self->field_54, func_08A5B6F8);
    int i = 0;
    A6Row *r = self->field_78;
    do {
        func_08A589CC(r);
        i++;
        r++;
    } while (i < 3);
    i = 0;
    A6Cell *c = self->field_1B0;
    do {
        func_08A5919C(c);
        i++;
        c++;
    } while (i < 5);
}
}

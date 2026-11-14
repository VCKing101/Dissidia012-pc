struct A6Res2 {
    char pad_000[0x400];
    char field_400[8];
    int field_408[3];
    char pad_414[0x470];
    char field_884[4];
};

struct A6ResOwner2 {
    A6Res2 *field_00;
};

extern "C" {
extern unsigned short D_08A88A40[];
void func_088D9F5C(int h, int v, int one);
void func_088D3F00(A6Res2 *a, char *b, void (*cb)(void));
void func_088D44EC(char *a, char *b, void (*cb)(void));
void func_08A889A0(void);
void func_08A889D0(void);

void func_08A883B4(A6ResOwner2 *self, int idx) {
    int i = 0;
    A6Res2 *r = self->field_00;
    int ofs = 0;
    do {
        func_088D9F5C(*(int *)((char *)r + ofs + 0x408), D_08A88A40[0], 1);
        i++;
        ofs += 4;
        r = self->field_00;
    } while (i < 3);
    func_088D9F5C(r->field_408[idx], D_08A88A40[1], 1);
    func_088D3F00(self->field_00, self->field_00->field_400, func_08A889A0);
    func_088D44EC(self->field_00->field_400, self->field_00->field_884, func_08A889D0);
}
}

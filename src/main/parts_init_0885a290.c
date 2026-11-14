typedef struct System0885A290 {
    unsigned char unknown_0000[0x160];
    unsigned char field_0160[0x14];
    unsigned char field_0174[0x1C];
    unsigned char field_0190[0x350];
    unsigned char field_04E0[0x20];
    unsigned char field_0500[0x2B0];
    unsigned char field_07B0[0x4B20];
    unsigned char field_52D0[0xF30];
    unsigned char field_6200[0x3E0];
    unsigned char field_65E0[0x3E0];
    unsigned char field_69C0[0x140];
    unsigned char field_6B00[4];
} System0885A290;

void func_08853EE4(System0885A290 *self);
void func_0885A5C8(void *p);
void func_08856170(void *p);
void func_08858668(void *p);
void func_08854950(void *p, int arg);
void func_08859084(void *p);
void func_0885AF4C(void *p);
void func_088577D8(void *p);
void func_08859D6C(void *p);
void func_08851C28(void *p);
void func_088569A4(void *p);
void func_08852550(void *p);

void func_0885A290(System0885A290 *self, int arg) {
    func_08853EE4(self);
    func_0885A5C8(self->field_0160);
    func_08856170(self->field_0174);
    func_08858668(self->field_0190);
    func_08854950(self->field_04E0, arg);
    func_08859084(self->field_0500);
    func_0885AF4C(self->field_07B0);
    func_088577D8(self->field_52D0);
    func_08859D6C(self->field_6200);
    func_08851C28(self->field_65E0);
    func_088569A4(self->field_69C0);
    func_08852550(self->field_6B00);
}

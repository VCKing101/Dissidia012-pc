struct FillEntry {
    char pad_00[3];
    unsigned char field_03;
};

struct FillList {
    char pad_0000[0x4C];
    char field_4C[0xDC];
    FillEntry *field_128[0x18];
    char pad_0188[0x1F98 - 0x188];
    int field_1F98;
    int pad_1F9C;
    int field_1FA0[0x18];
};

extern "C" int D_08A4FFF4;
extern "C" void func_088D4788(char *list, int *ids, int count);
extern "C" void func_08A4EEE4(int handle, int kind);

extern "C" {
void func_08A67648(FillList *self)
{
    if (self->field_1F98 == 1 || self->field_1F98 == 2) {
        int count = 0;
        for (int i = 0; i < 0x18; i++) {
            if (self->field_128[i] == 0) {
                break;
            }
            if (self->field_128[i]->field_03) {
                self->field_1FA0[count++] = i;
            }
        }
        func_088D4788(self->field_4C, self->field_1FA0, count);
    } else {
        for (int i = 0; i < 0x18; i++) {
            self->field_1FA0[i] = -1;
        }
        func_088D4788(self->field_4C, self->field_1FA0, 0);
    }
    func_08A4EEE4(D_08A4FFF4, self->field_1F98);
}
}

typedef struct NotifyTarget {
    char pad_00[0x20];
    unsigned short field_20;
} NotifyTarget;

typedef struct NotifyOwner {
    int field_00;
    NotifyTarget *field_04;
    int field_08;
} NotifyOwner;

extern void func_088D9F5C(NotifyTarget *target, int kind, int value);

void func_089EAEE4(NotifyOwner *self)
{
    if ((unsigned char)((self->field_04->field_20 & 0x20) != 0) == 1) {
        func_088D9F5C(self->field_04, 5, 1);
        self->field_08 = 1;
    }
}

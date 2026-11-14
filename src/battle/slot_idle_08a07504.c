extern int func_08A156C0(void *);

typedef struct Slot7504 {
    int field_00;
    int field_04;
    char pad_008[0x20];
    char field_28[4];
} Slot7504;

unsigned char func_08A07504(Slot7504 *self)
{
    int result = 0;
    if (self->field_04 != -1 || !func_08A156C0(self->field_28)) {
        result = 1;
    }
    return result;
}

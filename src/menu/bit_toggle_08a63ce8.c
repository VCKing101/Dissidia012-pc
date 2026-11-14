typedef struct BitObj {
    unsigned char pad_00[0x20];
    unsigned short field_20;
} BitObj;

typedef struct BitOwner {
    unsigned char field_00;
    char pad_01[0x2B];
    BitObj *field_2C;
} BitOwner;

extern void func_08A63CE8(BitOwner *self)
{
    if (self->field_00 != 0) {
        BitObj *obj = self->field_2C;
        obj->field_20 = obj->field_20 & ~0x4000;
        func_088D9D24(obj);
    } else {
        BitObj *obj = self->field_2C;
        obj->field_20 = obj->field_20 | 0x4000;
        func_088D9D24(obj);
    }
}

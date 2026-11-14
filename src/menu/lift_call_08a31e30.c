typedef struct LiftInner {
    char pad_00[0x3C];
    int field_3C;
} LiftInner;

typedef struct LiftOwner {
    char pad_00[0x48];
    LiftInner *field_48;
} LiftOwner;

extern void func_08834D30(float a, float b);

unsigned char func_08A31E30(LiftOwner *self)
{
    if (self->field_48->field_3C != 0) {
        func_08834D30(0.0f, 0.0f);
        return 1;
    }
    return 0;
}

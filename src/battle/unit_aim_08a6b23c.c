typedef struct Vec46B23C {
    float x;
    float y;
    float z;
    float w;
} Vec46B23C;

typedef struct UnitAim6B23C {
    char pad_000[0x68];
    int field_68;
} UnitAim6B23C;

extern int func_08A6AF6C(UnitAim6B23C *self, Vec46B23C *out);
extern int func_089A3B70(Vec46B23C *v);
extern void func_08A966AC(int id, Vec46B23C *v);

int func_08A6B23C(UnitAim6B23C *self)
{
    Vec46B23C direction;
    if (func_08A6AF6C(self, &direction)) {
        return -1;
    }
    if (!func_089A3B70(&direction)) {
        int id = self->field_68;
        func_08A966AC(id, &direction);
    }
    return 0;
}

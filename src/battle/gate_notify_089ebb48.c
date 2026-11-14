extern void func_088D9F5C(int, int, int);

typedef struct GateBB48 {
    unsigned char field_00;
    char pad_001[7];
    int field_08;
} GateBB48;


int func_089EBB48(GateBB48 *self)
{
    int result = -1;
    if (!self->field_00) {
        return result;
    }
    func_088D9F5C(self->field_08, -1, 1);
    return 0;
}

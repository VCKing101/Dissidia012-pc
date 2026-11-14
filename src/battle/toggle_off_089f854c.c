extern void func_089F7E40(void *);
extern void func_08889F70(int, int);

typedef struct Toggle854C {
    unsigned char field_00;
    char pad_001[3];
    int field_04;
    int field_08;
    unsigned char field_0C;
    char pad_00D[7];
    char field_14[4];
} Toggle854C;

int func_089F854C(Toggle854C *self)
{
    if (!self->field_00) {
        return -1;
    }
    if (!self->field_0C) {
        return -2;
    }
    func_089F7E40(self->field_14);
    func_08889F70(self->field_08, self->field_04 + 1);
    self->field_0C = 0;
    return 0;
}

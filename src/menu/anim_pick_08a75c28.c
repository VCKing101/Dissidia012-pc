typedef struct PickAnim {
    int field_00;
    char field_04[4];
    int field_08;
    int field_0C;
    unsigned char field_10;
} PickAnim;

extern void func_088D2FB8(PickAnim *self, int a, int b, int c);
extern void func_088D3450(char *anim, PickAnim *owner, int frames);

void func_08A75C28(PickAnim *self, int a, int b)
{
    func_088D2FB8(self, a, b, 3);
    if (self->field_10 == 1) {
        func_088D3450(self->field_04, self, self->field_0C);
    } else {
        func_088D3450(self->field_04, self, self->field_08);
    }
}

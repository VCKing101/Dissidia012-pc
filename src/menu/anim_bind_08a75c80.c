typedef struct BindAnim {
    int field_00;
    char field_04[0xC];
    unsigned char field_10;
} BindAnim;

extern void func_08A75CE4(BindAnim *self);
extern void func_088D321C(char *anim, int source);
extern void func_088D3068(BindAnim *self, char *anim);

void func_08A75C80(BindAnim *self, int source, unsigned char flag)
{
    func_08A75CE4(self);
    func_088D321C(self->field_04, source);
    func_088D3068(self, self->field_04);
    self->field_10 = flag;
}

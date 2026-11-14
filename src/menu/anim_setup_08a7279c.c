typedef struct AnimPart {
    int field_00;
    char field_04[0x20];
    char field_24[4];
} AnimPart;

extern void func_088D3278(char *anim, int a, int b, int c, int d, int e);
extern void func_088D3374(char *anim, int a, int b);
extern void func_088D3068(char *target, char *anim);
extern void func_088D3174(char *target);

void func_08A7279C(AnimPart *self, int a, int b, int c)
{
    char *anim = self->field_24;
    func_088D3278(anim, a, b, c, 0, 0);
    func_088D3374(anim, 1, 4);
    func_088D3068(self->field_04, anim);
    func_088D3174(self->field_04);
}

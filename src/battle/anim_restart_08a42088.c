typedef struct AnimState42088 {
    int mode;
    int looping;
    int value;
    char pad_0C[0x1B4];
    void *field_1C0;
    void *field_1C4;
} AnimState42088;

typedef struct AnimOwner42088 {
    char pad_00[0x14];
    AnimState42088 *state;
} AnimOwner42088;

extern void func_088D9CFC(void *anim);
extern void func_088D9F5C(void *anim, int motion, int restart);

void func_08A42088(AnimOwner42088 *self, int value)
{
    func_088D9CFC(self->state->field_1C0);
    func_088D9F5C(self->state->field_1C4, self->state->looping == 0 ? 2 : 5, 1);
    self->state->value = value;
    self->state->mode = 2;
}

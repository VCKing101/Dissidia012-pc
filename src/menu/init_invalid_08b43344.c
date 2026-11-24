/* Resets four halfwords to -1 and four floats to -1.0. */
typedef struct R { short s[4]; float f[4]; } R;
void func_08B43344(R *self)
{
    short m = -1;
    self->s[0] = m;
    self->s[1] = m;
    self->s[2] = m;
    self->s[3] = m;
    self->f[0] = -1.0f;
    self->f[1] = -1.0f;
    self->f[2] = -1.0f;
    self->f[3] = -1.0f;
}

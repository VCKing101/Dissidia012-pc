/* Runs func_08B55F50, func_08B55FDC(self), then sets self+0x50 = 1, +0x54 = 0.0f, +0x58 = 0, byte +0x5C = 0. */
struct Self { char pad[0x50]; int a; float b; int c; unsigned char d; };
extern void func_08B55F50(void);
extern void func_08B55FDC(struct Self *self);
void func_08B55E8C(struct Self *self)
{
    func_08B55F50();
    func_08B55FDC(self);
    self->a = 1;
    self->b = 0.0f;
    self->c = 0;
    self->d = 0;
}

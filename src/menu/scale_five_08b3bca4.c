/* Applies the same global scale to five sub-objects of the owner's block. */
typedef struct B3BCA4 { int w[0x34 / 4]; } B3BCA4;
typedef struct S3BCA4 { char pad[0xC8]; B3BCA4 *b; } S3BCA4;
extern float D_089CEC48;
extern void func_0892AD00(int, float);
void func_08B3BCA4(S3BCA4 *self)
{
    func_0892AD00(self->b->w[0x14 / 4], D_089CEC48);
    func_0892AD00(self->b->w[0x1C / 4], D_089CEC48);
    func_0892AD00(self->b->w[0x30 / 4], D_089CEC48);
    func_0892AD00(self->b->w[0x18 / 4], D_089CEC48);
    func_0892AD00(self->b->w[0x2C / 4], D_089CEC48);
}

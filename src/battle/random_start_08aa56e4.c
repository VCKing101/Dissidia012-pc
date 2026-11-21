/* Starts the effect: reseeds its state from the shared LCG and marks it active. */
typedef struct Rnd38 { unsigned char active; char pad[3]; int value; } Rnd38;
extern void func_08AA56C8(void *self);
extern int D_09AF84D0;
int func_08AA56E4(Rnd38 *self)
{
    func_08AA56C8(self);
    self->value = (D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    self->active = 1;
    return 0;
}

/* Loads five resources through the global loader into the object's handle slots and clears its state. */
typedef struct Handles { int h0; int h1; int h2; int h3; int h4; char pad_14[0x14]; int s28; int s2c; unsigned char s30; } Handles;
extern void *D_089D7830;
extern int func_088D0700(void *loader, int one, int id, int kind);
void func_08B16DF4(Handles *self)
{
    int one = 1;
    self->h3 = func_088D0700(D_089D7830, one, 0x22C, 3);
    self->h0 = func_088D0700(D_089D7830, one, 0x22A, 8);
    self->h1 = func_088D0700(D_089D7830, one, 0x22E, 9);
    self->h2 = func_088D0700(D_089D7830, one, 0x230, 10);
    self->h4 = func_088D0700(D_089D7830, one, 0x25C, -1);
    self->s28 = 0;
    self->s2c = 0;
    self->s30 = 0;
}

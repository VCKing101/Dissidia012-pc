/* Calls func_08B5301C(rec->0xC, rec->0, 10, D_09B6DAC0.h02, self->0xC + 1) with rec = self->0xC8; if the halfword at 0x09B6DB58 is below 2 calls func_08B53184(rec->0xC). */
struct Rec { int a; int pad; int pad2; int b; };
struct Self { char pad[0xC]; int n; char pad2[0xC8 - 0x10]; struct Rec *rec; };
struct Blk { char pad[2]; short h; char pad2[0x98 - 4]; short mode; };
extern struct Blk D_09B6DAC0;
extern void func_08B5301C(int b, int a, int ten, int h, int n);
extern void func_08B53184(int b);
void func_08B3B6F0(struct Self *self)
{
    func_08B5301C(self->rec->b, self->rec->a, 10, D_09B6DAC0.h, self->n + 1);
    if (D_09B6DAC0.mode < 2) { func_08B53184(self->rec->b); }
}

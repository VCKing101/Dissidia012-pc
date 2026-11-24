/* Sets a byte and two words, stores six callback addresses in the record and registers the sub-block. */
struct Rec { char pad[0x2B80]; char b; char p[3]; int w84; int w88; void *cb[6]; char *blk; };
extern int D_089D7260;
extern void func_08B58450(void);
extern void func_08B5845C(void);
extern void func_08B58440(void);
extern void func_08B58464(void);
extern void func_08B58478(void);
extern void func_08B58420(void);
extern void func_088B04AC(int, void *, void *, void *, void *);
void func_08B58080(struct Rec *self) {
    self->b = 0;
    self->w84 = 1;
    self->w88 = 0x1190;
    self->cb[0] = func_08B58450;
    self->cb[1] = func_08B5845C;
    self->cb[2] = func_08B58440;
    self->cb[3] = func_08B58464;
    self->cb[4] = func_08B58478;
    self->cb[5] = func_08B58420;
    func_088B04AC(D_089D7260, self, (char *)self + 0x2BA8, (char *)self + 0x2BA8, self);
}

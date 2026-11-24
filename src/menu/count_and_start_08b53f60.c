/* Increments the halfword at 0x089D8592 and, when the byte at self+0x140 is zero, calls func_08B53D24(self, -1). */
struct Cnt { short a, b; };
struct Self { char pad[0x140]; unsigned char busy; };
extern struct Cnt D_089D8590;
extern void func_08B53D24(struct Self *self, int v);
void func_08B53F60(struct Self *self)
{
    struct Cnt *c = &D_089D8590;
    c->b = c->b + 1;
    if (self->busy == 0) { func_08B53D24(self, -1); }
}

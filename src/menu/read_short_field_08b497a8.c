/* Parses src against the string at 0x08B4A258, stores its halfword at +8 through *self and returns src + 0x10. */
struct Dst { int v; };
struct Src { char pad[8]; short h; char pad2[6]; };
extern char D_08B4A258[];
extern void func_08935970(struct Src *s, char *fmt);
struct Src *func_08B497A8(struct Dst **self, struct Src *src)
{
    struct Dst **d = self;
    struct Src *s = src;
    func_08935970(s, D_08B4A258);
    (*d)->v = s->h;
    return s + 1;
}

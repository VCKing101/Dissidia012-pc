/* When func_0887E718(D_089D6998) says idle: calls func_08885220(D_089D69E8), func_088D3A04(self->0x138 + 4), the 0x08865158 hook on self+0x100; returns 0. */
struct Self { char pad[0x138]; char *p; };
extern "C" void *D_089D6998;
extern "C" void *D_089D69E8;
extern "C" int func_0887E718(void *p);
extern "C" void func_08885220(void *p);
extern "C" void func_088D3A04(char *p);
extern "C" void func_08865158(char *p);
struct Kept { Self *v; Kept(Self *s) { v = s; } };
extern "C" int func_08B1DE98(Self *self)
{
    Kept k(self);
    int r = func_0887E718(D_089D6998);
    self = k.v;
    if (r) { return 0; }
    k.v = self;
    func_08885220(D_089D69E8);
    func_088D3A04(k.v->p + 4);
    func_08865158((char *)k.v + 0x100);
    return 0;
}

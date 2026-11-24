/* Teardown: runs the sub-object's cleanup member, frees it and the handle, clearing both fields. */
struct Entry { short adj; short pad; void (*fn)(char *); };
struct Sub { char d[0x24]; char *table; };
struct Self { char d[0xBC]; int h; Sub *sub; };
struct Kept { Self *v; };
extern "C" void func_08B3B51C(Self *);
extern "C" void func_0885C488(Sub *);
extern "C" void func_08B42B88(int);
extern "C" void func_08B3B464(Self *self)
{
    Kept k[1];
    if (self->h == 0) return;
    k[0].v = self;
    func_08B3B51C(self);
    self = k[0].v;
    {
        Entry *e = (Entry *)(self->sub->table + 0x18);
        e->fn((char *)self->sub + e->adj);
    }
    self = k[0].v;
    func_0885C488(self->sub);
    self = k[0].v;
    int h = self->h;
    self->sub = 0;
    func_08B42B88(h);
    self = k[0].v;
    self->h = 0;
}

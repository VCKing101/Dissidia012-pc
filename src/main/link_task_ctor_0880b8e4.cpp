/* Link task constructor: builds the base task, then records the two linked
   objects and clears the four link slots. */
struct LinkTask0880B8E4 {
    char pad_000[0x2C];
    short state;
    char pad_02E[0x70 - 0x2E];
    void *vtable;
    char pad_074[0x80 - 0x74];
    void *target;
    char pad_084[0x208 - 0x84];
    void *source;
    int links[4];
};

extern "C" {

extern int D_089C6D18;

void func_08807860(LinkTask0880B8E4 *self, int a, int b, int c, int slot);

LinkTask0880B8E4 *func_0880B8E4(LinkTask0880B8E4 *self, int a, int b, int c, void *source, void *target)
{
    func_08807860(self, a, b, c, -1);
    self->vtable = &D_089C6D18;
    self->target = target;
    self->source = source;
    self->state = 1;
    self->links[0] = -1;
    self->links[1] = -1;
    self->links[2] = -1;
    self->links[3] = -1;
    return self;
}

}

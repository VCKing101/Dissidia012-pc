/* Allocates the screen state from `heap`, copies the setup values into it
   and resets it. */
struct Setup08B270D4 {
    int field_0;
    unsigned char field_4;
    char pad_5;
    unsigned short field_6;
    int field_8;
};

struct State08B270D4 {
    int field_000;
    char pad_004[0xE8 - 4];
    int field_0E8;
    char pad_0EC[0x1A4 - 0xEC];
    int field_1A4;
    char pad_1A8[0x1B0 - 0x1A8];
    int field_1B0;
    int field_1B4;
};

struct Owner08B270D4 {
    struct State08B270D4 *field_0;
    void *field_4;
};

extern struct State08B270D4 *func_0885C6CC(void *heap, int size, int a, int b, int c, int d);
extern void func_08B25070(struct Owner08B270D4 *self);

void func_08B270D4(struct Owner08B270D4 *self, void *heap, struct Setup08B270D4 *setup, int arg)
{
    int one = 1;
    self->field_4 = heap;
    self->field_0 = func_0885C6CC(heap, 0x2FF8, 0, 0, one, 0);
    self->field_0->field_0E8 = setup->field_8;
    self->field_0->field_000 = setup->field_0;
    self->field_0->field_1B4 = setup->field_6;
    self->field_0->field_1B0 = setup->field_4;
    self->field_0->field_1A4 = arg;
    func_08B25070(self);
}

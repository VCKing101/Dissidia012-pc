/* Creates an actor's body object and its two six-slot effect lists. */
struct Body088371C4 {
    char data[0xA4];
};

struct Slot088371C4 {
    char data[0x40];
};

struct Slots088371C4 {
    Slot088371C4 a[6];
    Slot088371C4 b[6];
    unsigned char count;
    unsigned char f301;
    unsigned char reset;
    unsigned char f303;
    char pad_304[0xC];
};

struct Actor088371C4 {
    char pad_000[0x3AC];
    Body088371C4 *body;
    char pad_3B0[0x534 - 0x3B0];
    Slots088371C4 *slots;
};

extern "C" {

void *func_088312D0(Actor088371C4 *self, int size);
void func_0883E054(Body088371C4 *body, Actor088371C4 *owner);
void func_08837190(Slot088371C4 *slot);
void func_08993560(void *array, int count, int size, void (*ctor)(Slot088371C4 *));

void func_088371C4(Actor088371C4 *self)
{
    Slots088371C4 *result2[1];
    Body088371C4 *result1[1];
    Body088371C4 *none1 = 0;
    result1[0] = none1;
    Body088371C4 *body = (Body088371C4 *)func_088312D0(self, sizeof(Body088371C4));
    Body088371C4 *made1 = result1[0];
    if (body != 0) {
        func_0883E054(body, self);
        made1 = body;
    }
    self->body = made1;
    Slots088371C4 *none2 = 0;
    result2[0] = none2;
    Slots088371C4 *slots = (Slots088371C4 *)func_088312D0(self, sizeof(Slots088371C4));
    Slots088371C4 *made2 = result2[0];
    if (slots != 0) {
        func_08993560(slots->a, 6, sizeof(Slot088371C4), func_08837190);
        func_08993560(slots->b, 6, sizeof(Slot088371C4), func_08837190);
        slots->f303 = 0;
        slots->reset = 0;
        slots->f301 = 0;
        slots->count = 0;
        made2 = slots;
    }
    self->slots = made2;
}

}

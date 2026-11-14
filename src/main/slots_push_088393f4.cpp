/* Adds an entry to a six-slot list, clearing the list first when a reset
   is pending. */
struct Slot088393F4 {
    char pad_00[0x20];
    int a;
    int b;
    int c;
    int d;
    short e;
    short scale;
    unsigned char f;
    unsigned char g;
    short id;
    char pad_38[8];
};

struct Slots088393F4 {
    Slot088393F4 slots[6];
    char pad_180[0x300 - 0x180];
    unsigned char count;
    char pad_301;
    unsigned char reset;
};

extern "C" {

void func_088395B8(Slot088393F4 *slot, int a, int b, int c);

void func_088393F4(Slots088393F4 *self, int a, int b, int c)
{
    if (self->reset) {
        self->count = 0;
        for (int i = 0; i < 6; i++) {
            Slot088393F4 *s = &self->slots[i];
            s->a = 0;
            s->b = 0;
            s->c = 0;
            s->d = 0;
            s->e = 0;
            s->scale = 100;
            s->f = 0;
            s->g = 0;
            s->id = -1;
        }
        self->reset = 0;
    }
    if (self->count < 6) {
        func_088395B8(&self->slots[self->count], a, b, c);
        self->count++;
    }
}

}

/* Sets or clears the timer's ready flag from the item's charge time. */
struct Charge08842C30 {
    short time;

    static int get(Charge08842C30 *c) { return c->time; }
};

struct Item08842C30 {
    int field_00;
    Charge08842C30 charge;
};

struct Timer08842C30 {
    char pad_00[8];
    int start;
    int now;
    char pad_10[0x18];
    unsigned int flags;
};

extern "C" {

void *func_08835798(int id);
Item08842C30 *func_088BF4CC(void *table, int key);
int func_08842F48(int elapsed, int now, int time);

void func_08842C30(Timer08842C30 *t, int id)
{
    Item08842C30 *item = func_088BF4CC(func_08835798(id), 0x154);
    int time = 0;
    if (item != 0) {
        Charge08842C30 *charge = &item->charge;
        time = charge->time;
    }
    int elapsed = t->now - t->start;
    elapsed = elapsed < 0 ? 0 : elapsed;
    if (func_08842F48(elapsed, t->now, time) == 1) {
        unsigned int flags = t->flags | 4;
        t->flags = flags;
    } else {
        unsigned int flags = t->flags & ~4;
        t->flags = flags;
    }
}

}

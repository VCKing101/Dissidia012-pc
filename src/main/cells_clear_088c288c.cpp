/* Empties all 100 cells (id -1, count 0) and the used counter. */
struct Cell088C288C { signed char id; unsigned char count; };
struct Board088C288C { Cell088C288C cells[100]; int used; };
extern "C" {
void func_088C288C(Board088C288C *b)
{
    for (int i = 0; i < 100; i++) {
        Cell088C288C *c = &b->cells[i];
        c->id = -1;
        c->count = 0;
    }
    b->used = 0;
}
}

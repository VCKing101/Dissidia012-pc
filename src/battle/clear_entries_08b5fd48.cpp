// Zeroes the d field of each of the first count entries, then clears count.
struct Ent { int a; int b; int c; int d; };
struct Tbl { Ent e[20]; int count; };
extern "C" void func_08B5FD48(Tbl *self)
{
    for (int i = 0, n = self->count; i < n; i++) {
        self->e[i].c = 0;
    }
    self->count = 0;
}

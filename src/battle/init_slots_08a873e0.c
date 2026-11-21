/* Stores four non-null slot pointers in the object and activates it; -1 bad argument, -2/-3 rejected. */
typedef struct Slots38 { unsigned char active; char pad[3]; int a; int b; int c; int d; } Slots38;
extern void func_08A87328(Slots38 *self);
extern int func_08A87B58(Slots38 *self, int a);
extern int func_08A87BEC(Slots38 *self, int a);
int func_08A873E0(Slots38 *self, int a, int b, int c, int d)
{
    func_08A87328(self);
    if (b == 0 || a == 0 || c == 0 || d == 0) {
        return -1;
    }
    if (func_08A87B58(self, a) != 0) {
        return -2;
    }
    if (func_08A87BEC(self, a) != 0) {
        return -3;
    }
    self->a = a;
    self->b = b;
    self->c = c;
    self->d = d;
    self->active = 1;
    return 0;
}

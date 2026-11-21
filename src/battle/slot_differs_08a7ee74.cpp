/* Returns whether the two slot indexes of the entry map to different table values. */
struct Slot38 { unsigned char active; char pad[0xF]; unsigned a; unsigned pad2; unsigned b; };
extern "C" {
extern unsigned D_08A7FFE0[];
bool func_08A7EE74(Slot38 *p)
{
    if (p->active == 0) {
        return false;
    }
    if (p->a >= 7) {
        return false;
    }
    if (p->b >= 7) {
        return true;
    }
    return D_08A7FFE0[p->a * 3] != D_08A7FFE0[p->b * 3];
}
}

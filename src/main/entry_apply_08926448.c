/* Applies each of the (up to) group-0xE entries of the object found for the given id: forwards its two words (8 and 0xC) to func_089261E0. */
typedef struct E08926448 { char pad[8]; int a; int b; } E08926448;
int func_08926048(int owner, int id);
int func_0885C9A4(int g, int kind);
E08926448 *func_0885CA3C(int g, int kind, int i);
void func_089261E0(int owner, int target, int kind, int a, int b);
void func_08926448(int owner, int target, short id)
{
    int g = func_08926048(owner, id);
    int n = func_0885C9A4(g, 0xE);
    int i;
    for (i = 0; i < n; i++) {
        E08926448 *e = func_0885CA3C(g, 0xE, i);
        if (e == 0) {
            break;
        }
        func_089261E0(owner, target, 0xE, e->a, e->b);
    }
}

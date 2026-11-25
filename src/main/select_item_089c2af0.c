// Plays sound 9 and starts an effect for the item, or stops sound 9 when the item has no id.
extern void func_089A0384(int id);
extern void func_089A03DC(int id);
extern void func_08973334(int a, int b, int c, int d, int e, int f);
struct Item089C2AF0 { int pad; int id; };
void func_089C2AF0(int unused, int a, struct Item089C2AF0 *it)
{
    if (it->id != -1) {
        func_089A0384(9);
        func_08973334(a, it->id, 5, 1, 0, 0);
    } else {
        func_089A03DC(9);
    }
}

// Plays sound 9, notifies the owner of the selected item and starts an effect, or stops sound 9 when the item has no id.
extern void func_089A0384(int id);
extern void func_089A03DC(int id);
extern void func_08973508(int a, int id, float x, float y);
extern void func_08973334(int a, int b, int c, int d, int e, int f);
struct Owner089C2B5C { int pad; float a; float b; };
struct Item089C2B5C { int pad; int id; };
void func_089C2B5C(struct Owner089C2B5C *o, int a, struct Item089C2B5C *it)
{
    if (it->id != -1) {
        func_089A0384(9);
        func_08973508(a, it->id, o->a, o->b);
        func_08973334(a, it->id, 5, 1, 0, 0);
    } else {
        func_089A03DC(9);
    }
}

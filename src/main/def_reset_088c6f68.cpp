/* Resets a definition: ids, state, both links to -1 and both pairs. */
struct Pair088C6F68 { short a; short b; };
struct Def088C6F68 { short id; char pad[0x18]; short state; char pad2[0x14]; short links[2]; Pair088C6F68 pairs[2]; };
extern "C" {
void func_088C6F54(Pair088C6F68 *p);
void func_088C6F68(Def088C6F68 *d)
{
    d->id = 0;
    d->state = 0;
    for (int i = 0; i < 2; i++) {
        d->links[i] = -1;
        func_088C6F54(&d->pairs[i]);
    }
}
}

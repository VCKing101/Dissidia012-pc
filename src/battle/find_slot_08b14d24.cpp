// Searches the 32-entry table at D_08B15638 (stride 0xC) for an entry whose first word equals a1; returns it or 0.
struct Ent08B14D24 { int key; int a; int b; };
extern Ent08B14D24 D_08B15638[];
extern "C" Ent08B14D24 *func_08B14D24(int a0, int key)
{
    for (unsigned i = 0; i < 0x20; i++) {
        if (D_08B15638[i].key == key) {
            return &D_08B15638[i];
        }
    }
    return 0;
}

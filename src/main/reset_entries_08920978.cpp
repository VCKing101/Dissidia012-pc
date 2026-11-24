/* For each of the five ids at +0x18 that are in range 1..34, looks up its entry and clears two fields. */
extern int D_089D712C;
struct Entry { char pad[6]; short s6; char pad2[4]; int w; };
extern "C" Entry *func_088CB164(int tbl, int id);
extern "C" void func_08920978(int *self)
{
    for (int i = 0; i < 5; i++) {
        int id = self[6 + i];
        if (id <= 0 || id >= 0x23) continue;
        Entry *e = func_088CB164(D_089D712C + 0xF58, id);
        e->w = 0;
        e->s6 = 0;
    }
}

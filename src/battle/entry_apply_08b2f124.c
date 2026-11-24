/* Calls func_088355AC, then if the entry looked up by key has a positive count byte at +0x1F, forwards it to func_08ACF2A0. */
typedef struct CEntry { char pad[0x1F]; unsigned char count; } CEntry;
typedef struct CSelf { char pad[0xD5C]; char list[1]; } CSelf;
extern void func_088355AC(void);
extern CEntry *func_088BBD8C(int key);
extern void func_08ACF2A0(void *list, CSelf *self, int count);
void func_08B2F124(CSelf *self, int key)
{
    CEntry *e;
    unsigned char n;
    func_088355AC();
    e = func_088BBD8C(key);
    n = e->count;
    if (0 < n) {
        func_08ACF2A0(self->list, self, n);
    }
}

/* Looks up an entry and forwards its low-nibble kind to func_08B0D5B4; 0 when missing or kind 0xF. */
typedef struct KindEntry { char pad_00[0x1E]; unsigned char kind; } KindEntry;
extern KindEntry *func_088BBD8C(int key);
extern int func_08B0D5B4(void *self, int kind, int arg);
int func_08B0D514(void *self, int key, int arg)
{
    KindEntry *e = func_088BBD8C(key);
    if (e != 0) {
        int kind = e->kind & 0xF;
        if (kind != 0xF) {
            return func_08B0D5B4(self, kind, arg);
        }
        return 0;
    }
    return 0;
}

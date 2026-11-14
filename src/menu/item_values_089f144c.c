/* Screen0B6A0: computes both values of item `index` (func_089F050C for
   its two entries), scales the first by the kind-6 bonus percentage and
   doubles both under flag 0x80000 (which also forces kind 8). */
typedef struct ItemRow29 {
    char pad_00[0x10];
    short entry1;   /* +0x10 */
    short entry2;   /* +0x12 */
    short value1;   /* +0x14 */
    short value2;   /* +0x16 */
} ItemRow29;

typedef struct ItemList29 {
    unsigned char kind_set;
    char pad_01[0x3];
    unsigned int flags;
    char pad_08[0x4];
    ItemRow29 rows[1];  /* +0x0C */
} ItemList29;

extern int func_089F050C(void *self, ItemList29 *items, short entry, unsigned char bonus, int index,
                         int kind);
extern int func_089EFB50(void *self, unsigned int kind, int index);

void func_089F144C(void *self, ItemList29 *list, int index, unsigned char bonus)
{
    int kind = -1;
    int percent;

    if ((list->flags & 0x80000) != 0) {
        kind = 8;
    }
    list->rows[index].value1 = func_089F050C(self, list, list->rows[index].entry1, bonus, index, kind);
    percent = func_089EFB50(self, 6, -1);
    if ((list->flags & 2) == 0 && percent != 0) {
        list->rows[index].value1 = list->rows[index].value1 * percent / 100;
    }
    list->rows[index].value2 = func_089F050C(self, list, list->rows[index].entry2, bonus, index, kind);
    if ((list->flags & 0x80000) != 0) {
        list->rows[index].value1 *= 2;
        list->rows[index].value2 *= 2;
    }
}

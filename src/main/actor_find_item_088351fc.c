typedef struct Item { unsigned char id; } Item;
typedef struct ItemTable { char pad_00[0x18]; void *list; } ItemTable;
typedef struct Actor088350AC { char pad_000[0x390]; ItemTable *table; } Actor088350AC;
extern Item *func_08811FAC(void *list, int key, Item *after);
extern unsigned char func_08837D18(Actor088350AC *a, int id);
Item *func_088351FC(Actor088350AC *a, int key, Item *after)
{
    if (a->table->list == 0) {
        return 0;
    }
    do {
        after = func_08811FAC(a->table->list, key, after);
    } while (after != 0 && func_08837D18(a, after->id) == 0);
    return after;
}

/* Motion queries: the frame count of a fighter motion by slot (and, for
 * the second, by slot and variant); -1 closed, -2 bad slot, -3 no table,
 * -4 no motion, -5 not loaded. */
typedef struct MotionEntry {
    unsigned char pad_00[0xC];
    short frames;
} MotionEntry;

typedef struct MotionBank {
    unsigned char pad_00[4];
    void *table;
} MotionBank;

typedef struct MotionQuery {
    unsigned char open;
    unsigned char pad_01[3];
    MotionBank *bank;
} MotionQuery;

extern int func_08B33B6C(void *table, unsigned int slot, int kind);
extern MotionEntry *func_088B1D9C(int id);

int func_08A651A8(MotionQuery *self, unsigned int slot, int kind)
{
    void *table;
    int id;
    MotionEntry *entry;
    if (!self->open) {
        return -1;
    }
    if (slot >= 12) {
        return -2;
    }
    table = self->bank->table;
    if (table == 0) {
        return -3;
    }
    id = func_08B33B6C(table, slot, kind);
    if (id < 0) {
        return -4;
    }
    entry = func_088B1D9C(id);
    if (entry == 0) {
        return -5;
    }
    return entry->frames;
}

extern int func_08B33BD4(void *table, unsigned int slot, int kind, unsigned int variant);

int func_08A65248(MotionQuery *self, unsigned int slot, unsigned int variant, int kind)
{
    void *table;
    int id;
    MotionEntry *entry;
    if (!self->open) {
        return -1;
    }
    if (slot >= 12 || variant >= 3) {
        return -2;
    }
    table = self->bank->table;
    if (table == 0) {
        return -3;
    }
    id = func_08B33BD4(table, slot, kind, variant);
    if (id < 0) {
        return -4;
    }
    entry = func_088B1D9C(id);
    if (entry == 0) {
        return -5;
    }
    return entry->frames;
}

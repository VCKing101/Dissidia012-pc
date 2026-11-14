typedef struct ScaleEntry {
    char pad_00[0x48];
    int base;
} ScaleEntry;

typedef struct ScaleOwner {
    char pad_00[0x4];
    int table;
} ScaleOwner;

extern ScaleEntry *func_08ABC300(int table, int id);
extern float func_08ABDD94(ScaleOwner *self, int b);
extern float func_08ABDDB0(ScaleOwner *self, int a);

int func_08ABDC60(ScaleOwner *self, int a, int b)
{
    ScaleEntry *entry;
    float first;
    float second;
    if (a == 0) {
        return 0;
    }
    entry = func_08ABC300(self->table, 12);
    if (entry == 0) {
        return 0;
    }
    first = func_08ABDD94(self, b);
    second = func_08ABDDB0(self, a);
    return (int)((float)entry->base * second * first);
}

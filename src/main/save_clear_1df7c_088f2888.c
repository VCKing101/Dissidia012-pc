typedef struct SaveCounters {
    short first;
    short second;
} SaveCounters;

typedef struct SaveData {
    char pad_00000[0x1DF7C];
    SaveCounters counters;
} SaveData;

extern SaveData *D_089D712C;

void func_088F2888(void)
{
    SaveCounters *c = &D_089D712C->counters;

    c->first = 0;
    c->second = 0;
}

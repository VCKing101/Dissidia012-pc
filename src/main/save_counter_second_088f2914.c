typedef struct SaveCounters {
    short first;
    short second;
} SaveCounters;

typedef struct SaveData {
    char pad_00000[0x1DF7C];
    SaveCounters counters;
} SaveData;

extern SaveData *D_089D712C;

int func_088F2914(void)
{
    SaveCounters *c = &D_089D712C->counters;
    int value = c->second;

    if (value < 0) {
        value = 0;
    }
    return value;
}

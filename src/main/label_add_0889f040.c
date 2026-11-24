/* Adds a label record and its (a, b) slot with a formatted name; returns
   false when the 256-label table is full. */
typedef struct LabelRecord {
    int words[11];
} LabelRecord;

typedef struct LabelCounter {
    char pad_00[8];
    int count;
} LabelCounter;

typedef struct LabelSlot {
    short a;
    short b;
    char name[0x80];
} LabelSlot;

typedef struct LabelTable {
    char pad_00[4];
    LabelCounter *counter;
    LabelRecord *records;
    char pad_0C[0x40 - 0xC];
    int base;
    LabelSlot *slots;
} LabelTable;

extern int D_089D6998;
extern char D_089CAB70[];

extern void func_0893476C(char *buf, const char *fmt, int a, int b);
extern void func_0887DF14(int text, char *out, int size, const char *key);

int func_0889F040(LabelTable *self, LabelRecord *record, short a, short b)
{
    char key[64];

    if (self->counter->count < 0x100) {
        int index;
        self->records[self->counter->count] = *record;
        index = self->counter->count - self->base;
        self->slots[index].a = a;
        self->slots[index].b = b;
        func_0893476C(key, D_089CAB70, a, b);
        {
            LabelSlot *slot = &self->slots[index];
            func_0887DF14(D_089D6998, slot->name, 0x80, key);
        }
        self->counter->count++;
        return 1;
    }
    return 0;
}

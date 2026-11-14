typedef struct LookupEntry {
    char pad_00[0xC];
    short field_0C;
} LookupEntry;

typedef struct LookupList {
    int field_00;
    LookupEntry *field_04[1];
} LookupList;

typedef struct LookupBuffer {
    char data[0x40];
} LookupBuffer;

extern void func_088DA0A8(LookupEntry *entry, int kind, LookupBuffer *out, int *extra);
extern void func_0887B778(void *target, LookupBuffer *buffer);
extern void func_0887B828(void *target, int *extra);

void func_08A1FCBC(void *self, LookupList *list, void **targets, int index, int slot)
{
    if (list->field_04[index]->field_0C != -1) {
        if (targets[slot] != 0) {
            LookupBuffer buffer;
            int extra;
            func_088DA0A8(list->field_04[index], 0, &buffer, &extra);
            func_0887B778(targets[slot], &buffer);
            func_0887B828(targets[slot], &extra);
        }
    }
}

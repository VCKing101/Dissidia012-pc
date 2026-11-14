/* Releases effects 1-11 and clears the nine effect ids. */
typedef struct EffectTable {
    char pad_000[0xEF0];
    void *effects[12];
    char pad_F20[0xF6A - 0xF20];
    unsigned short ids[9];
} EffectTable;

typedef struct EffectOwner {
    char pad_00[0x30];
    EffectTable *table;
} EffectOwner;

extern void func_088FC87C(EffectOwner *owner, void *effect);
extern void func_0887BA5C(void *effect);

void func_088FBC08(EffectOwner *owner)
{
    int i;

    for (i = 1; i < 12; i++) {
        if (owner->table->effects[i] != 0) {
            func_088FC87C(owner, owner->table->effects[i]);
            if (owner->table->effects[i] != 0) {
                func_0887BA5C(owner->table->effects[i]);
                owner->table->effects[i] = 0;
            }
        }
    }
    for (i = 0; i < 9; i++) {
        owner->table->ids[i] = 0xFFFF;
    }
}

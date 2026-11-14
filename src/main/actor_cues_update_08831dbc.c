typedef struct CueEntry { char pad_00[0x50]; short id; char pad_52[0xE]; } CueEntry;
typedef struct CueTable { CueEntry *entries; int count; } CueTable;
typedef struct CueBody { char pad_00[0x84]; CueTable *table; } CueBody;
typedef struct Actor08831DBC { char pad_000[0x394]; CueBody *body; char pad_398[0x528 - 0x398]; int *states; } Actor08831DBC;
extern void func_0886A9B0(CueBody *body, int index, int id, int flags);
void func_08831DBC(Actor08831DBC *a)
{
    int i;
    int n = a->body->table->count;
    for (i = 0; i < n; i++) {
        int *state = &a->states[i];
        if (*state == 0) {
            continue;
        }
        switch (*state) {
        case 1:
            func_0886A9B0(a->body, i, a->body->table->entries[i].id, -1);
            a->states[i] = 0;
            break;
        case 2:
            *state = 1;
            break;
        }
    }
}

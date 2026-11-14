/* True when the 0x5BC-byte team entry `index` has a unit. */
typedef struct TeamEntry {
    void *unit;
    char pad_04[0x5BC - 4];
} TeamEntry;

typedef struct Team {
    char pad_000[0x438];
    TeamEntry entries[2];
} Team;

int func_088F2D8C(Team *team, int index)
{
    TeamEntry *entry = &team->entries[index];

    if (entry->unit != 0) {
        return 1;
    }
    return 0;
}

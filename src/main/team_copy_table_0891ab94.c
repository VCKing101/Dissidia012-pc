typedef struct TeamTable {
    int words[90];
} TeamTable;

typedef struct TeamData {
    char pad_000[0x5D4];
    TeamTable table;            /* 0x5D4 */
} TeamData;

void func_0891AB94(TeamData *team, TeamTable *src)
{
    team->table = *src;
}

extern "C" {
extern void func_0891A0F4(void *slot);
extern void func_0891A15C(void *link);

typedef struct TeamEntry {
    char data[0x2E8];
} TeamEntry;

typedef struct Team {
    int field_000;
    TeamEntry entries[2];       /* 0x004 */
    char link[0x20];            /* 0x5D4 */
} Team;

void func_0891A848(Team *team)
{
    for (int i = 0; i < 2; i++) {
        func_0891A0F4(&team->entries[i]);
    }
    func_0891A15C(team->link);
}
}

typedef struct MenuState {
    char pad_00[0x58];
    int team;                   /* 0x58 */
} MenuState;

typedef struct TeamSlot {
    char data[0x280];
} TeamSlot;

extern char *D_089D712C;
extern MenuState *D_089D83D8;

TeamSlot *func_0891705C(int index)
{
    if (index >= 0) {
        return (TeamSlot *)(D_089D712C + index * sizeof(TeamSlot) + 0x3E8C8);
    }
    return (TeamSlot *)(D_089D712C + D_089D83D8->team * sizeof(TeamSlot) + 0x3E8C8);
}

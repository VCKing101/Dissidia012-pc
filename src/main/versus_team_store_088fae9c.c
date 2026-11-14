/* Copies a 0x138-byte team record into slot `index` of the versus screen. */
typedef struct VersusTeam {
    int words[0x138 / 4];
} VersusTeam;

typedef struct VersusState {
    char pad_0000[0x20C8];
    VersusTeam teams[2];
} VersusState;

extern VersusState D_09AFB8A0;

void func_088FAE9C(int index, VersusTeam *team)
{
    int teams = (int)D_09AFB8A0.teams;

    *(VersusTeam *)(index * (int)sizeof(VersusTeam) + teams) = *team;
}

/* With flag 4 set, sends each of the five members' rank to the board. */
typedef struct RankMember {
    int field_00;
    int field_04;
} RankMember;

typedef struct RankState {
    char pad_0000[0x788];
    RankMember field_788[5];
    char pad_07B0[0x7CC0 - 0x7B0];
    unsigned int field_7CC0;
    char pad_7CC4[0x7CCC - 0x7CC4];
    void *field_7CCC;
} RankState;

typedef struct RankOwner {
    char pad_0000[0x7CFC];
    RankState *field_7CFC;
} RankOwner;

extern int func_08AB1CDC(int value);
extern void func_08AB51F4(void *board, int index, int rank);

void func_08AA9120(RankOwner *self)
{
    int i;
    if ((self->field_7CFC->field_7CC0 & 4) == 0) {
        return;
    }
    for (i = 0; i < 5; i++) {
        if (self->field_7CFC->field_788[i].field_00 > 0) {
            int rank = func_08AB1CDC(self->field_7CFC->field_788[i].field_00);
            func_08AB51F4(self->field_7CFC->field_7CCC, i, rank);
        }
    }
}

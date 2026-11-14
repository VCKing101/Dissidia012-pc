/* Whether the board accepts a move to `target` from the current (or first) position. */
struct MoveCheckState {
    char pad_0[0x4];
    int field_04;
};

struct MoveCheckBoard {
    char pad_0000[0x7CA4];
    MoveCheckState *field_7CA4;
    char pad_7CA8[0x7CC0 - 0x7CA8];
    unsigned int field_7CC0;
    char pad_7CC4[0x7CCC - 0x7CC4];
    void *field_7CCC;
};

struct MoveCheckOwner {
    char pad_0000[0x7CFC];
    MoveCheckBoard *field_7CFC;
};

extern "C" int func_08AB47BC(void *board);
extern "C" int func_08AB619C(void *board, int from, int target);
extern "C" int func_08AB624C(void *board, int player, int target);

extern "C" {
bool func_08AAA608(MoveCheckOwner *self, int target)
{
    int from;
    if ((self->field_7CFC->field_7CC0 & 1) == 0) {
        return false;
    }
    from = func_08AB47BC(self->field_7CFC->field_7CCC);
    if (from == -1) {
        from = 0;
    }
    if (func_08AB619C(self->field_7CFC->field_7CCC, from, target) == 1) {
        return false;
    }
    if (func_08AB624C(self->field_7CFC->field_7CCC, self->field_7CFC->field_7CA4->field_04, target) == 0) {
        return false;
    }
    return true;
}
}

typedef struct IdPair { int first; int second; } IdPair;
typedef struct CardState { char pad_00[0xE]; unsigned char phase; } CardState;
IdPair func_08928F3C(CardState *state)
{
    IdPair pair;
    pair.first = -1;
    pair.second = -1;
    if (state->phase == 2) {
        pair.first = 0;
    }
    return pair;
}

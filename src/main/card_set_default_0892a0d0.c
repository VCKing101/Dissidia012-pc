typedef struct CardSet { char pad_00[0x1A]; short values[21]; } CardSet;
extern int D_089CEB88[];
void func_0892A0D0(CardSet *set, int index)
{
    set->values[index] = D_089CEB88[(unsigned int)index];
}

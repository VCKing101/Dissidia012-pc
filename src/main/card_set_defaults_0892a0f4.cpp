extern "C" {
typedef struct CardSet { char pad_00[0x1A]; short values[21]; } CardSet;
extern void func_0892A0D0(CardSet *set, int index);
void func_0892A0F4(CardSet *set)
{
    for (int i = 0; i < 21; i++) {
        func_0892A0D0(set, i);
    }
}
}

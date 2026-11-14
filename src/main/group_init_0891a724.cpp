extern "C" {
extern void func_0891A6F8(void *card);

typedef struct Card {
    char data[0x14];
} Card;

typedef struct CardGroup {
    Card cards[5];
    int count;                  /* 0x64 */
} CardGroup;

void func_0891A724(CardGroup *group)
{
    for (int i = 0; i < 5; i++) {
        func_0891A6F8(&group->cards[i]);
    }
    group->count = 0;
}
}

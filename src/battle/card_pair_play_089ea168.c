/* Plays a card on the pair: checks the request and forwards it to the card
 * of the requested index. */
typedef struct CardItem {
    unsigned char data[0x84C];
} CardItem;

typedef struct CardPair {
    unsigned char open;
    unsigned char pad_01[3];
    void *context;
    unsigned char shared[0x10];
    void *back;
    CardItem items[2];
} CardPair;

typedef struct CardPlay {
    int index;
    unsigned char pad_04[0x24];
    int forced;
} CardPlay;

extern int func_089EA11C(CardPair *pair);
extern int func_089EA91C(CardItem *item, CardPlay *play);
extern unsigned char D_089D5F1F;

int func_089EA168(CardPair *pair, CardPlay *play)
{
    if (func_089EA11C(pair) == 0) {
        return -1;
    }
    if (play == 0) {
        return -2;
    }
    if (play->index >= 2) {
        return -3;
    }
    if (play->forced == 0 && D_089D5F1F == 1) {
        return 0;
    }
    if (func_089EA91C(&pair->items[play->index], play) != 0) {
        return -4;
    }
    return 0;
}

/* Opens a card pair: loads the shared back model from the owner's card
 * source, then opens both cards. */
struct CardItem {
    unsigned char data[0x84C];
};

struct CardOwner {
    unsigned char pad_000[0x1C4];
    void *source;
};

struct CardPair {
    unsigned char open;
    unsigned char pad_01[3];
    void *context;
    unsigned char shared[0x10];
    void *back;
    CardItem items[2];
};

extern "C" {

extern void func_089E9F04(CardPair *pair);
extern void *func_088DAA40(void *source, int name, int flags);
extern int func_089EA7B0(CardItem *item, void *context, int index, void *shared, void *source);

int func_089E9F74(CardPair *pair, CardOwner *owner, void *context)
{
    func_089E9F04(pair);
    if (owner == 0 || owner->source == 0 || context == 0) {
        return -1;
    }
    pair->back = func_088DAA40(owner->source, 13, 0);
    if (pair->back == 0) {
        return -2;
    }
    for (int i = 0; i < 2; i++) {
        if (func_089EA7B0(&pair->items[i], context, i, pair->shared, owner->source) != 0) {
            return -3;
        }
    }
    pair->context = context;
    pair->open = 1;
    return 0;
}

}

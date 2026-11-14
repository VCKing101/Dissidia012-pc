extern "C" {
/* Updates both markers unless the game is paused. */
typedef struct Item08872DEC {
    char data[0x40];
} Item08872DEC;

typedef struct Owner08872DEC {
    char pad_00[0x160];
    Item08872DEC items[2];
} Owner08872DEC;

typedef struct Game08872DEC {
    char pad_000[0x264];
    unsigned int flags;
} Game08872DEC;

extern Game08872DEC *D_089D6008;

void func_08872B04(Item08872DEC *item);

void func_08872DEC(Owner08872DEC *o)
{
    if (D_089D6008->flags & 1) {
        return;
    }
    for (int i = 0; i < 2; i++) {
        func_08872B04(&o->items[i]);
    }
}
}

extern "C" {
/* Updates both gauges unless the game is paused. */
typedef struct Item08872D88 {
    char data[0x50];
} Item08872D88;

typedef struct Owner08872D88 {
    char pad_00[0xC0];
    Item08872D88 items[2];
} Owner08872D88;

typedef struct Game08872D88 {
    char pad_000[0x264];
    unsigned int flags;
} Game08872D88;

extern Game08872D88 *D_089D6008;

void func_088725A0(Item08872D88 *item);

void func_08872D88(Owner08872D88 *o)
{
    if (D_089D6008->flags & 1) {
        return;
    }
    for (int i = 0; i < 2; i++) {
        func_088725A0(&o->items[i]);
    }
}
}

/* Player value at 0x1C when the player is ready; -1 otherwise. */
typedef struct Player088CD9FC {
    char pad_00[0x1C];
    int value;
} Player088CD9FC;

int func_088CD9D4(Player088CD9FC *p);
int func_088CD9FC(Player088CD9FC *p)
{
    if (func_088CD9D4(p) == 0) {
        return -1;
    }
    return p->value;
}

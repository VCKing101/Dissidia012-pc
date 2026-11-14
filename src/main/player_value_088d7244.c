/* Player value at 0xC when the player is ready; 0 otherwise. */
typedef struct Player088D7244 {
    char pad_00[0xC];
    int value;
} Player088D7244;

int func_088D723C(Player088D7244 *p);
int func_088D7244(Player088D7244 *p)
{
    if (func_088D723C(p) == 0) {
        return 0;
    }
    return p->value;
}

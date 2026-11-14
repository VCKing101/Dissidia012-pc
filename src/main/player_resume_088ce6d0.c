/* Clears the voice's bit 5 when the player is ready; -1 otherwise. */
typedef struct Voice088CE6D0 {
    char pad_00[0x14];
    unsigned char flags;
} Voice088CE6D0;

typedef struct Player088CE6D0 {
    int field_00;
    Voice088CE6D0 *voice;
} Player088CE6D0;

int func_088CE71C(Player088CE6D0 *p);
int func_088CE6D0(Player088CE6D0 *p)
{
    if (func_088CE71C(p) == 0) {
        return -1;
    }
    p->voice->flags &= ~0x20;
    return 0;
}

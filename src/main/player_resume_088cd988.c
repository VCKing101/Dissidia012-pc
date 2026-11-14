/* Clears the voice's bit 5 when the player is ready; -1 otherwise. */
typedef struct Voice088CD988 {
    char pad_00[0x14];
    unsigned char flags;
} Voice088CD988;

typedef struct Player088CD988 {
    int field_00;
    Voice088CD988 *voice;
} Player088CD988;

int func_088CD9D4(Player088CD988 *p);
int func_088CD988(Player088CD988 *p)
{
    if (func_088CD9D4(p) == 0) {
        return -1;
    }
    p->voice->flags &= ~0x20;
    return 0;
}

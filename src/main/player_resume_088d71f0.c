/* Clears the voice's bit 5 when the player is ready; -1 otherwise. */
typedef struct Voice088D71F0 {
    char pad_00[0x14];
    unsigned char flags;
} Voice088D71F0;

typedef struct Player088D71F0 {
    int field_00;
    int field_04;
    int field_08;
    Voice088D71F0 *voice;
} Player088D71F0;

int func_088D723C(Player088D71F0 *p);
int func_088D71F0(Player088D71F0 *p)
{
    if (func_088D723C(p) == 0) {
        return -1;
    }
    p->voice->flags &= ~0x20;
    return 0;
}

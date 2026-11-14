/* Marks the player armed when it is ready; -1 otherwise. */
typedef struct Player088CE080 {
    char pad_00[8];
    unsigned char armed;
} Player088CE080;

int func_088CE2A0(Player088CE080 *p);

int func_088CE080(Player088CE080 *p)
{
    if (func_088CE2A0(p) == 0) {
        return -1;
    }
    p->armed = 1;
    return 0;
}

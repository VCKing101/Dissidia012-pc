/* Marks the player armed when it is ready; -1 otherwise. */
typedef struct Player088D73B4 {
    char pad_00[0x1C];
    unsigned char armed;
} Player088D73B4;

int func_088D723C(Player088D73B4 *p);

int func_088D73B4(Player088D73B4 *p)
{
    if (func_088D723C(p) == 0) {
        return -1;
    }
    p->armed = 1;
    return 0;
}

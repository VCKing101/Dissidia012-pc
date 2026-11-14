/* Resets the holder and stores three non-null arguments, marking it active. */
typedef struct Setup3 {
    unsigned char active;
    char pad_01[0x10 - 1];
    void *first;
    void *second;
    void *third;
} Setup3;

extern void func_088D7BF0(Setup3 *holder);

int func_088D7C30(Setup3 *holder, void *first, void *second, void *third)
{
    func_088D7BF0(holder);
    if (first == 0 || second == 0 || third == 0) {
        return -1;
    }
    holder->first = first;
    holder->second = second;
    holder->third = third;
    holder->active = 1;
    return 0;
}

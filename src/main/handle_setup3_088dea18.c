typedef struct Setup3B {
    unsigned char active;
    char pad_01[7];
    void *first;
    void *second;
    void *third;
} Setup3B;

extern void func_088DE9D8(Setup3B *holder);

int func_088DEA18(Setup3B *holder, void *first, void *second, void *third)
{
    func_088DE9D8(holder);
    if (first == 0 || second == 0 || third == 0) {
        return -1;
    }
    holder->first = first;
    holder->second = second;
    holder->third = third;
    holder->active = 1;
    return 0;
}

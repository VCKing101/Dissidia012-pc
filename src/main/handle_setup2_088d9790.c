typedef struct Setup2 {
    unsigned char active;
    char pad_01[7];
    void *first;
    void *second;
} Setup2;

extern void func_088D9750(Setup2 *holder);

int func_088D9790(Setup2 *holder, void *first, void *second)
{
    func_088D9750(holder);
    if (first == 0 || second == 0) {
        return -1;
    }
    holder->first = first;
    holder->second = second;
    holder->active = 1;
    return 0;
}

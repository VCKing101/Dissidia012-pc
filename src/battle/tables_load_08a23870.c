typedef struct TablePair {
    char pad_00[0x10];
    char first[0x70];
    char second[0x4];
} TablePair;

extern void func_08A23930(TablePair *self);
extern int func_089EED04(void *table, void *data, int count);

/* Loads both tables; -1 bad arguments, -2/-3 when a table fails to load. */
int func_08A23870(TablePair *self, void *first, int firstCount, void *second, int secondCount)
{
    func_08A23930(self);
    if (first == 0 || firstCount <= 0 || second == 0 || secondCount <= 0) {
        return -1;
    }
    if (func_089EED04(self->first, first, firstCount) != 0) {
        return -2;
    }
    if (func_089EED04(self->second, second, secondCount) != 0) {
        return -3;
    }
    return 0;
}

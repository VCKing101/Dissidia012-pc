/* Definition for step i of a set: the last, the first, or one of the extra middle steps; 0 otherwise. */
typedef struct Def088C72E0 { char data[0x3C]; } Def088C72E0;
typedef struct DefSet088C72E0 {
    char pad_00[6];
    short count;
    Def088C72E0 first;
    Def088C72E0 last;
    Def088C72E0 defs[3];
    char pad_134[0x134 - 0x80 - 3 * 0x3C];
    int extra;
} DefSet088C72E0;
Def088C72E0 *func_088C72E0(DefSet088C72E0 *set, int i)
{
    int n = set->count;
    Def088C72E0 *result = 0;
    if (i == n - 1) {
        result = &set->last;
    } else if (i == 0) {
        result = &set->first;
    } else if (i < n - 1 && i >= n - set->extra - 1) {
        unsigned int slot = i - n + 4;
        result = (Def088C72E0 *)((char *)set->defs + slot * sizeof(Def088C72E0));
    }
    return result;
}

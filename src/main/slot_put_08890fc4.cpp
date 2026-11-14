extern "C" {
/* Puts a value in the first free (negative) of two slots: -1 when inactive,
   -2 for a negative value. */
typedef struct Owner08890FC4 {
    unsigned char active;
    char pad_01[0xB];
    int slots[2];
} Owner08890FC4;

int func_08890FC4(Owner08890FC4 *o, int value)
{
    if (!o->active) {
        return -1;
    }
    if (value < 0) {
        return -2;
    }
    for (int i = 0; i < 2; i++) {
        if (o->slots[i] >= 0) {
            continue;
        }
        o->slots[i] = value;
        break;
    }
    return 0;
}
}

/* True when the 7-bit id at 5 is set and equals the current id. */
typedef struct IdHolder {
    char pad_00[5];
    unsigned char id;
} IdHolder;

extern int func_08867888(void);

int func_088DFCA4(IdHolder *holder)
{
    if (holder->id == 0) {
        return 0;
    }
    if (func_08867888() == (holder->id & 0x7F)) {
        return 1;
    }
    return 0;
}

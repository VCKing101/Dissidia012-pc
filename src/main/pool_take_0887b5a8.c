/* Takes the first free id of the 4-entry pool, or 0 when all are in use. */
typedef struct Entry0887B5A8 {
    int id;
    unsigned char used;
    char pad_05[3];
} Entry0887B5A8;

typedef struct Pool0887B5A8 {
    int count;
    int field_04;
    Entry0887B5A8 entries[4];
} Pool0887B5A8;

extern Pool0887B5A8 D_09B48C60;

int func_0887B5A8(void)
{
    int id = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (D_09B48C60.entries[i].used == 0) {
            id = D_09B48C60.entries[i].id;
            D_09B48C60.entries[i].used = 1;
            D_09B48C60.count++;
            break;
        }
    }
    return id;
}

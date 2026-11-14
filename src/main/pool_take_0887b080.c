/* Takes the first free id of the 8-entry pool, or 0 when all are in use. */
typedef struct Entry0887B080 {
    int id;
    unsigned char used;
    char pad_05[3];
} Entry0887B080;

typedef struct Pool0887B080 {
    int count;
    int field_04;
    Entry0887B080 entries[8];
} Pool0887B080;

extern Pool0887B080 D_09B48C18;

int func_0887B080(void)
{
    int id = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (D_09B48C18.entries[i].used == 0) {
            id = D_09B48C18.entries[i].id;
            D_09B48C18.entries[i].used = 1;
            D_09B48C18.count++;
            break;
        }
    }
    return id;
}

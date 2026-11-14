/* Returns id to the 4-entry pool. */
typedef struct Entry0887B5F8 {
    int id;
    unsigned char used;
    char pad_05[3];
} Entry0887B5F8;

typedef struct Pool0887B5F8 {
    int count;
    int field_04;
    Entry0887B5F8 entries[4];
} Pool0887B5F8;

extern Pool0887B5F8 D_09B48C60;

void func_0887B5F8(int id)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (D_09B48C60.entries[i].id == id) {
            if (D_09B48C60.entries[i].used) {
                D_09B48C60.entries[i].used = 0;
            }
            D_09B48C60.count--;
            return;
        }
    }
}

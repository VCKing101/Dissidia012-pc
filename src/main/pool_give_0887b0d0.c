/* Returns id to the 8-entry pool. */
typedef struct Entry0887B0D0 {
    int id;
    unsigned char used;
    char pad_05[3];
} Entry0887B0D0;

typedef struct Pool0887B0D0 {
    int count;
    int field_04;
    Entry0887B0D0 entries[8];
} Pool0887B0D0;

extern Pool0887B0D0 D_09B48C18;

void func_0887B0D0(int id)
{
    int i;
    for (i = 0; i < 8; i++) {
        if (D_09B48C18.entries[i].id == id) {
            if (D_09B48C18.entries[i].used) {
                D_09B48C18.entries[i].used = 0;
            }
            D_09B48C18.count--;
            return;
        }
    }
}

/* Fills `out` with the loaded entries of the five-slot table (with their
   sizes) when `all` is set, otherwise asks func_088D5AC8; returns the count. */
struct Out08AF42C4 {
    void *field_0;
    unsigned short field_4;
    unsigned short field_6;
};

struct Table08AF42C4 {
    char pad_00[0xC];
    void *field_C[5];
};

extern struct Table08AF42C4 D_09B6CDA0;
extern int func_088D5AC8(struct Out08AF42C4 *out, int arg, int a, int b);
extern unsigned short func_088D5E0C(void *entry);

int func_08AF42C4(int unused, struct Out08AF42C4 *out, int arg, unsigned char all)
{
    int count = 0;
    int i;
    if (all) {
        for (i = 0; i < 5; i++) {
            if (D_09B6CDA0.field_C[i] != 0) {
                out[count].field_0 = D_09B6CDA0.field_C[i];
                out[count].field_4 = 0;
                out[count].field_6 = func_088D5E0C(out[count].field_0);
                count++;
            }
        }
    } else {
        count = func_088D5AC8(out, arg, 0, 0);
    }
    return count;
}

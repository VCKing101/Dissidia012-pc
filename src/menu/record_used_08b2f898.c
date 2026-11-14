/* True when record `id` has any of its three links set (not negative). */
struct Record08B2F898 {
    char pad_0[0xC];
    short field_C[3];
};

extern struct Record08B2F898 *func_088C5DF0(int id);

unsigned char func_08B2F898(void *self, int id)
{
    struct Record08B2F898 *record = func_088C5DF0(id);
    int count = 0;
    int i = 0;
    short *links = record->field_C;
    for (; i < 3; i++) {
        if (links[i] >= 0) {
            count++;
        }
    }
    if (count == 0) {
        return 0;
    } else {
        return 1;
    }
}

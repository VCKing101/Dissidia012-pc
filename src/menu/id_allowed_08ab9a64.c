/* Whether the entry's kind is allowed: always without the filter, else when it is in the list. */
typedef struct AllowPage {
    char pad_000[0xD58];
    unsigned char field_D58;
} AllowPage;

typedef struct AllowOwner {
    char pad_000[0x138];
    AllowPage *field_138;
} AllowOwner;

extern int D_08ABB91C[];
extern int func_088F45DC(void *entry);

int func_08AB9A64(AllowOwner *self, void *entry)
{
    int result;
    if (self->field_138->field_D58 != 0) {
        int kind;
        int *id;
        result = 0;
        kind = func_088F45DC(entry);
        id = D_08ABB91C;
        for (;;) {
            if (*id == -1) {
                break;
            }
            if (*id == kind) {
                result = 1;
                break;
            }
            id++;
        }
    } else {
        result = 1;
    }
    return result;
}

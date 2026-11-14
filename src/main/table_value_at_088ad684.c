/* Value of the index-th entry of the -1-terminated table at 0x089D6F58; -1 past the end. */
typedef struct Entry088AD684 {
    int id;
    int field_04;
    int value;
} Entry088AD684;

extern Entry088AD684 D_089D6F58[];

int func_088AD684(unsigned int index)
{
    int result = -1;
    unsigned int i = 0;
    Entry088AD684 *e = D_089D6F58;
    for (;;) {
        if (e->id == -1) {
            break;
        }
        if (index == i) {
            result = e->value;
        }
        e++;
        i++;
    }
    return result;
}

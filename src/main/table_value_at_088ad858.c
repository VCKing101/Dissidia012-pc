/* Value of the index-th entry of the -1-terminated table at 0x089D70FC; -1 past the end. */
typedef struct Entry088AD858 {
    int id;
    int field_04;
    int value;
} Entry088AD858;

extern Entry088AD858 D_089D70FC[];

int func_088AD858(unsigned int index)
{
    int result = -1;
    unsigned int i = 0;
    Entry088AD858 *e = D_089D70FC;
    for (;;) {
        if (e->id == -1) {
            break;
        }
        if (index == i) {
            result = e->value;
            break;
        }
        e++;
        i++;
    }
    return result;
}

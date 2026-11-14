typedef struct Entry { char name[0x48]; } Entry;
typedef struct Book { char data_000[0x1A4]; char name[0x868 - 0x1A4]; unsigned int count; char data_86C[0x8A4 - 0x86C]; Entry entries[1]; } Book;
extern int func_08935B50(const char *a, const char *b, int size);

int func_088ABEFC(Book *b)
{
    unsigned int i;
    for (i = 0; i < b->count; i++) {
        if (func_08935B50(b->name, b->entries[i].name, 0x14) == 0) {
            return i;
        }
    }
    return -1;
}

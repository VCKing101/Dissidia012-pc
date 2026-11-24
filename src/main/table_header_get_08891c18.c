/* Copies the header of table entry `id` into `out`; returns 0, -1 for bad
   arguments or -2 when the entry does not exist. */
typedef struct TableHeader {
    int words[0x58 / 4];
} TableHeader;

typedef struct Table Table;

extern TableHeader *func_088C6CA4(int id);

int func_08891C18(Table *self, TableHeader *out, int id)
{
    TableHeader *header;

    if (out == 0 || id < 0) {
        return -1;
    }
    header = func_088C6CA4(id);
    if (header == 0) {
        return -2;
    }
    *out = *header;
    return 0;
}

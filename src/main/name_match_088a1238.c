typedef struct Entry088A1238 {
    int field_00;
    int field_04;
    char name[0x20];
} Entry088A1238;

typedef struct Key088A1238 {
    int field_00;
    const char *name;
} Key088A1238;

int func_08935B50(const char *a, const char *b, int length);

int func_088A1238(Key088A1238 *key, Entry088A1238 *entry) {
    if (func_08935B50(entry->name, key->name, 0x1F) == 0) {
        return 1;
    }
    return 0;
}

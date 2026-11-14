typedef struct Entry0887B1B4 {
    int offset;
    int size;
} Entry0887B1B4;

typedef struct Header0887B1B4 {
    unsigned char unknown_00[0xA];
    unsigned short count;
    unsigned char unknown_0C[4];
    Entry0887B1B4 entries[1];
} Header0887B1B4;

typedef struct Archive0887B1B4 {
    Header0887B1B4 *header;
} Archive0887B1B4;

char *func_0887B1B4(Archive0887B1B4 *archive, int index) {
    Header0887B1B4 *header = archive->header;
    char *result = 0;
    if (index < header->count) {
        Entry0887B1B4 *entries = header->entries;
        result = (char *)header + entries[index].offset;
    }
    return result;
}

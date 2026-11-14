/* Finds the chunk whose low id byte matches and sets the data cursor past the chunk table. */
typedef struct ChunkHeader {
    unsigned short field_00;
    unsigned short field_02;
} ChunkHeader;

typedef struct ChunkEntry {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
    unsigned short field_06;
} ChunkEntry;

extern ChunkHeader *D_08A3CCF4;
extern ChunkEntry *D_08A3CCF8;
extern char *D_08A3CCFC;

ChunkEntry *func_08A3C360(unsigned char id)
{
    int offset = 0;
    int found = 0;
    int i = 0;
    int count = D_08A3CCF4->field_02;
    ChunkEntry *chunks = D_08A3CCF8;
    for (i = 0; i < count; i++) {
        ChunkEntry *chunk = &chunks[i];
        if ((chunk->field_00 & 0xFF) == id) {
            found = i;
            break;
        }
        offset += chunk->field_04;
    }
    D_08A3CCFC = (char *)&chunks[count] + offset;
    return &chunks[found];
}

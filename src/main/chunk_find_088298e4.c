/* Returns the chunk (reached through the offset table) whose id matches, or 0. */
typedef struct Chunks088298E4 {
    int field_00;
    unsigned int count;
    int offsets[1];
} Chunks088298E4;

unsigned short *func_088298E4(Chunks088298E4 *chunks, unsigned int id)
{
    unsigned int i;
    for (i = 0; i < chunks->count; i++) {
        int *offset = &chunks->offsets[i];
        unsigned short *chunk = (unsigned short *)((char *)chunks + *offset);
        if (*chunk == id) {
            return chunk;
        }
    }
    return 0;
}

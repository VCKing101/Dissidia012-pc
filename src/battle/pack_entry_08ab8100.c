typedef struct PackHeader {
    short pad_00;
    short count;
    int offsets[1];
} PackHeader;

typedef struct PackRef {
    PackHeader *header;
} PackRef;

/* Entry number index of the pack, 0 past the end. */
short *func_08AB8100(PackRef *self, int index)
{
    PackHeader *header = self->header;
    if (index >= header->count) {
        return 0;
    }
    return (short *)((char *)header + header->offsets[index]);
}

/* Bit 0 of a marker's flag word. */
typedef struct MarkerFlags {
    char pad_00[0xA];
    short flags;                /* 0x0A */
} MarkerFlags;

int func_0891F624(MarkerFlags *m)
{
    return m->flags & 1;
}

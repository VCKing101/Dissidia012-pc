/* Whether bit 1 of a marker's flag word is set. */
typedef struct MarkerFlags {
    char pad_00[0xA];
    short flags;                /* 0x0A */
} MarkerFlags;

int func_0891F630(MarkerFlags *m)
{
    return (m->flags & 2) != 0;
}

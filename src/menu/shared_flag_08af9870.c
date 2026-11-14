/* Returns the shared lock byte. */
typedef struct FlagShared {
    char pad_000[0x9BC];
    unsigned char field_9BC;
} FlagShared;

extern FlagShared D_09B6CDA0;

unsigned char func_08AF9870(void)
{
    FlagShared *shared = &D_09B6CDA0;
    return shared->field_9BC;
}

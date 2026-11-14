/* Returns the shared block after its 8-byte header. */
typedef struct SharedHead {
    char pad_00[0x8];
    char field_08[0x10];
} SharedHead;

extern SharedHead D_09B6CDA0;

char *func_08AF34A0(void)
{
    SharedHead *shared = &D_09B6CDA0;
    return shared->field_08;
}

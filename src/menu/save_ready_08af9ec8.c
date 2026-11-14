/* Whether the shared save block is ready (always when it is not in use). */
typedef struct ReadyShared {
    char pad_000[0xF0];
    char field_F0[0x10];
} ReadyShared;

extern ReadyShared D_09B6CDA0;
extern int func_0892A1E0(void *block);
extern int func_0892A264(void *block);

int func_08AF9EC8(void)
{
    int result = 1;
    ReadyShared *shared = &D_09B6CDA0;
    char *block = shared->field_F0;
    if (func_0892A1E0(block) != 0) {
        result = func_0892A264(block);
    }
    return result;
}

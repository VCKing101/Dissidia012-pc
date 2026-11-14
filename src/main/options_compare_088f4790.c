/* Compares 0x78 bytes of options with the saved ones. */
typedef struct SaveData {
    char pad_00000[0x1DE04];
    unsigned char options[0x78];
} SaveData;

extern SaveData *D_089D712C;
extern int func_08935490(const void *a, const void *b, int size);

int func_088F4790(const void *options)
{
    return func_08935490(D_089D712C->options, options, 0x78);
}

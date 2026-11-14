/* Stores option byte `index`. */
typedef struct SaveData {
    char pad_00000[0x1DE04];
    unsigned char options[0x78];
} SaveData;

extern SaveData *D_089D712C;

void func_088F4870(int index, unsigned char value)
{
    unsigned char *options = D_089D712C->options;

    *((unsigned char *)index + (int)options) = value;
}

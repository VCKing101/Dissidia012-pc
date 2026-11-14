typedef struct SaveData {
    char pad_00000[0x1DE8A];
    unsigned char field_1DE8A;
} SaveData;

extern SaveData *D_089D712C;

void func_088F1270(int value)
{
    D_089D712C->field_1DE8A = value;
}

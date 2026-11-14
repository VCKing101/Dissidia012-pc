typedef struct SaveData {
    char pad_00000[0x1DE8F];
    unsigned char field_1DE8F;
} SaveData;

extern SaveData *D_089D712C;

void func_088F145C(int value)
{
    D_089D712C->field_1DE8F = value;
}

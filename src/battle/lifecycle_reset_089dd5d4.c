/* Clears the battle setup root header fields. */
typedef struct BattleRootHeader {
    unsigned char field_00;
    unsigned char padding_01[3];
    int field_04;
    int field_08;
    unsigned char field_0C[3];
    unsigned char field_0F;
} BattleRootHeader;

void func_089DD5D4(BattleRootHeader *header)
{
    int i;

    header->field_00 = 0;
    header->field_04 = 0;
    header->field_08 = 0;
    for (i = 0; i < 3; i++) {
        header->field_0C[i] = 0;
    }
    header->field_0F = 0;
}

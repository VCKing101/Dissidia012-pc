/* Minimum field views for the aggregate byte and the two target flag bytes. */
typedef struct BattleFlagTarget14 {
    unsigned char padding[0x14];
    unsigned char field_14;
} BattleFlagTarget14;
typedef struct BattleFlagOwner16C {
    unsigned char padding[0x16C];
    BattleFlagTarget14 *field_16C;
    unsigned char padding_170[0x28];
    BattleFlagTarget14 *field_198;
} BattleFlagOwner16C;
typedef struct BattleSlotAggregate {
    unsigned char padding_00[4];
    BattleFlagOwner16C *field_04;
    unsigned char padding_08[0x1B0];
    unsigned char field_1B8[3];
    unsigned char field_1BB;
} BattleSlotAggregate;

int func_089E381C(BattleSlotAggregate *object)
{
    unsigned int active = 0;
    int i;
    for (i = 0; i < 3; ++i) {
        if (object->field_1B8[i] == 1) {
            active = 1;
            break;
        }
    }
    if (object->field_1BB != active) {
        if (!active) {
            object->field_04->field_198->field_14 |= 1;
            object->field_04->field_16C->field_14 |= 1;
        } else {
            object->field_04->field_198->field_14 &= ~1;
            object->field_04->field_16C->field_14 &= ~1;
        }
        object->field_1BB = active;
    }
    return 0;
}

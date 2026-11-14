/* Whether the selected member (0-4) is ready. */
typedef struct ReadyMemberSlot {
    char pad_000[0x1BC];
    char field_1BC[0x164];
} ReadyMemberSlot;

typedef struct ReadyMemberPanel {
    char pad_000[0x1E8];
    int field_1E8;
    char pad_1EC[0x210 - 0x1EC];
    char **field_210;
} ReadyMemberPanel;

extern int func_0892A1E0(void *member);

int func_08A6C290(ReadyMemberPanel *self)
{
    int index = self->field_1E8;
    if (index >= 0 && index < 5 && func_0892A1E0(*self->field_210 + index * 0x164 + 0x1BC) != 0) {
        return 1;
    }
    return 0;
}

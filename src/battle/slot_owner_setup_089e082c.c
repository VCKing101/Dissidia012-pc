/* Sets up the 0x1C6C subobject: shared resource, four slots, binding. */
typedef struct BattleSlot1C {
    unsigned char raw[0x1C];
} BattleSlot1C;

typedef struct BattleSlotOwner {
    unsigned char field_00;
    unsigned char padding_01[3];
    void *field_04;
    unsigned char field_08[0x10];
    BattleSlot1C field_18[4];
    unsigned char field_88[0xC];
} BattleSlotOwner;

extern void *D_089D7830;
extern void func_089E07AC(BattleSlotOwner *);
extern void *func_088D09D8(void *, int, int);
extern int func_089E12A8(BattleSlot1C *, int, void *, void *);
extern int func_089E0D70(void *, void *);

int func_089E082C(BattleSlotOwner *owner)
{
    func_089E07AC(owner);
    owner->field_04 = func_088D09D8(D_089D7830, 1, 0x15);
    if (!owner->field_04) return -1;
    {
        int i;
        for (i = 0; i < 4; i++) {
            if (func_089E12A8(&owner->field_18[i], i, owner->field_04, owner->field_08)) return -2;
        }
    }
    if (func_089E0D70(owner->field_88, owner->field_04)) return -3;
    owner->field_00 = 1;
    return 0;
}

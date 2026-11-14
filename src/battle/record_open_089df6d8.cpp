#include "battle/agent1_types.h"

/* Opens one of the pool's three fixed records for an owner. */
extern "C" {

extern void func_089DF628(BattleRecord54 *record);
extern unsigned int func_089E9628(unsigned int index);
extern void *func_088DAA40(void *source, int name, int flags);
extern int D_089E0618[3];
extern int func_089DFF50(BattleRecord54 *record);

int func_089DF6D8(BattleRecord54 *record, unsigned int index, BattleRecordOwner *owner, int models, int start)
{
    func_089DF628(record);
    if (index >= 3 || owner == 0 || models == 0) {
        return -1;
    }
    void *source = owner->field_174;
    if (source == 0) {
        return -2;
    }
    void *kept[1];
    kept[0] = source;
    unsigned int kind = func_089E9628(index);
    source = kept[0];
    if (kind >= 3) {
        return -3;
    }
    record->field_2C = func_088DAA40(source, D_089E0618[kind], 0);
    if (record->field_2C == 0) {
        return -4;
    }
    record->field_44 = index;
    record->field_48 = owner;
    record->field_4C = models;
    record->field_18 = 0;
    record->field_14 = 0;
    record->field_10 = 0;
    record->field_08 = start;
    record->field_04 = start;
    record->field_00 = 1;
    if (func_089DFF50(record) != 0) {
        func_089DF628(record);
        return -5;
    }
    return 0;
}

}

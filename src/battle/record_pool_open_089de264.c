#include "battle/agent1_types.h"

/* Opens the record pool for an owner: binds the model set to the owner's
 * models and opens the three fixed records. */
extern void func_089DE1D8(BattleRecordPool *pool);
extern int func_089DF1F0(void *models, void *source);
extern int func_089DF6D8(BattleRecord54 *record, int index, BattleRecordOwner *owner, void *models, int kind);
extern int func_089E978C(int index);

int func_089DE264(BattleRecordPool *pool, BattleRecordOwner *owner, int context)
{
    func_089DE1D8(pool);
    if (owner == 0 || context == 0) {
        return -1;
    }
    if (owner->field_174 == 0) {
        return -2;
    }
    if (func_089DF1F0(pool->field_0C, owner->field_174) != 0) {
        return -3;
    }
    {
        int i;
        for (i = 0; i < 3; i++) {
            int kind = func_089E978C(i);
            if (func_089DF6D8(&pool->field_24[i], i, owner, pool->field_0C, kind) != 0) {
                return -4;
            }
        }
    }
    pool->field_04 = (int)owner;
    pool->field_08 = context;
    pool->field_00 = 1;
    return 0;
}

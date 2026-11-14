#include "battle/agent1_types.h"

/* Record pool accessors: per-record forwarders with range checks. */
extern void func_089E84E4(void *);
extern int func_089DF880(BattleRecord54 *);
extern float func_089DF8A0(BattleRecord54 *);
extern int func_089DF968(BattleRecord54 *, unsigned int, int, int);
extern int func_089DFB68(BattleRecord54 *, int);
extern int func_089DFBF8(BattleRecord54 *, int);
extern int func_089DFD68(BattleRecord54 *, unsigned int, unsigned char);

int func_089DE3AC(BattleRecordPool *pool)
{
    BattleListLink *link;

    if (!pool->field_00) return -1;
    link = 0;
    while ((link = !link ? pool->field_630 : link->next) != 0) {
        func_089E84E4(link);
    }
    return 0;
}

int func_089DE418(BattleRecordPool *pool, unsigned int index)
{
    BattleRecord54 *records;

    if (!pool->field_00) return 0;
    if (index >= 3) return 0;
    records = pool->field_24;
    return func_089DF880(&records[index]);
}

float func_089DE478(BattleRecordPool *pool, unsigned int index)
{
    BattleRecord54 *records;

    if (!pool->field_00) return 0.0f;
    if (index >= 3) return 0.0f;
    records = pool->field_24;
    return func_089DF8A0(&records[index]);
}

int func_089DE4D8(BattleRecordPool *pool, unsigned int index, unsigned int slot, int a, int b)
{
    if (!pool->field_00) return -1;
    if (index >= 3 || slot >= 4) return -2;
    return func_089DF968(&pool->field_24[index], slot, a, b);
}

int func_089DE568(BattleRecordPool *pool, unsigned int index, int value)
{
    if (!pool->field_00) return -1;
    if (index >= 3) return -2;
    if (func_089DFB68(&pool->field_24[index], value)) return -3;
    return 0;
}

int func_089DE5E4(BattleRecordPool *pool, unsigned int index, int value)
{
    if (!pool->field_00) return -1;
    if (index >= 3) return -2;
    if (func_089DFBF8(&pool->field_24[index], value)) return -3;
    return 0;
}

int func_089DE660(BattleRecordPool *pool, unsigned int index, unsigned int slot, unsigned char value)
{
    if (!pool->field_00) return -1;
    if (index >= 3 || slot >= 3) return -2;
    if (func_089DFD68(&pool->field_24[index], slot, value)) return -3;
    return 0;
}

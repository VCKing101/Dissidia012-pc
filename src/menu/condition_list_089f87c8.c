/* Screen0B6A0: lists the conditions (+0x0F kind, +0x10 amount) of the
   kind 6 entries not yet done (flag 0x8000) that `owner` meets.
   Standalone: the condition fields of the entries are not in the region
   header's Entry84. */
typedef struct CondEntry29 {
    unsigned char kind;
    char pad_01[0x3];
    unsigned int flags;
    char pad_08[0x7];
    signed char cond_kind; /* +0x0F */
    short need;            /* +0x10 */
    char pad_12[0x72];
} CondEntry29;

typedef struct CondWork29 {
    char pad_0000[0x2731];
    signed char count;     /* +0x2731 */
    char pad_2732[0x29A];
    CondEntry29 entries[9]; /* +0x29CC */
} CondWork29;

typedef struct CondScreen29 {
    char pad_000[0x14C];
    CondWork29 *work;
} CondScreen29;

typedef struct Condition29 {
    short kind;
    short need;
} Condition29;

typedef struct ConditionList29 {
    Condition29 items[10];
    short count; /* +0x28 */
} ConditionList29;

extern void func_0891A254(ConditionList29 *list);
extern int func_089F86EC(CondScreen29 *self, char kind, short need, void *owner);

void func_089F87C8(CondScreen29 *self, ConditionList29 *list, void *owner)
{
    int i;
    int n;
    short need;
    short cond;

    func_0891A254(list);
    for (i = 0; i < self->work->count; i++) {
        if (self->work->entries[i].kind == 6 && (self->work->entries[i].flags & 0x8000) == 0) {
            if (func_089F86EC(self, self->work->entries[i].cond_kind, self->work->entries[i].need, owner) != 0) {
                cond = self->work->entries[i].cond_kind;
                n = list->count;
                list->items[n].kind = cond;
                need = self->work->entries[i].need;
                list->count = n + 1;
                list->items[n].need = need;
            }
        }
    }
}

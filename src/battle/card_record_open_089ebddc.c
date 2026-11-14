#include "battle/agent28_types.h"

/* Opens a card record from a card definition. */
void func_089EBCC8(CardRecord *record);

int func_089EBDDC(CardRecord *record, void *owner, void *source, CardDef *def)
{
    int i;

    func_089EBCC8(record);
    if (owner == 0 || source == 0 || def == 0) {
        return -1;
    }
    if (def->side >= 2 || def->slot < 0) {
        return -2;
    }
    record->state = 0;
    record->owner = owner;
    record->source = source;
    record->side = def->side;
    record->slot = def->slot;
    record->kind = def->kind;
    record->style = def->style;
    record->value = def->value;
    for (i = 0; i < 3; i++) {
        record->params[i] = def->params[i];
    }
    record->mark = def->mark;
    record->timer = def->timer;
    record->sound = def->sound;
    record->voice = def->voice;
    record->extra = def->extra;
    record->open = 1;
    return 0;
}

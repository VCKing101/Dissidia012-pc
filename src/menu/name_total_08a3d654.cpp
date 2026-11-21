#include "menu/agent36_types.h"

/* Name list screen: total of the records' values for kind over the records
 * that count (func_08A3DD10), capped at 999999999. */

extern "C" int func_08A3DD10(NameScreen36 *self, NameRecord36 *record, int kind);

extern "C" int func_08A3D654(NameScreen36 *self, int kind)
{
    NameWork36 *work;
    NameRecord36 *record;
    int total;
    int i;

    work = self->work;
    total = 0;
    for (i = 0; i < work->record_count; i++) {
        record = &work->records[i];
        if (func_08A3DD10(self, record, kind) == 0) {
            work = self->work;
            continue;
        }
        total += record->values[kind];
        if (total > 999999999) {
            total = 999999999;
            break;
        }
        work = self->work;
    }
    return total;
}

extern "C" {
#include "battle/runtime_helpers.h"
void func_088DA060(void *);
}

/* Offset-only views for the two pointer fields used by the cleanup. */
struct BattleGridEntryView {
    unsigned char padding_00[0xC];
    void *field_0C;
    void *field_10;
};
struct BattleGridOps {
    static void clear_entry(BattleGridEntryView *entry) {
        if (entry->field_10) {
            func_088DA060(entry->field_10);
            entry->field_10 = 0;
        }
    }
    static void clear_header(BattleGridEntryView *entry) {
        if (entry->field_0C) {
            func_088DA060(entry->field_0C);
            entry->field_0C = 0;
        }
    }
};
extern "C" int func_089E439C(void *object)
{
    {
        int row_index = 0;
        int column;
        unsigned char *row = (unsigned char *)object;
        do {
            column = 0;
            unsigned char *entry = row;
            do {
                BattleGridOps::clear_entry((BattleGridEntryView *)entry);
                ++column;
                entry += 4;
            } while (column < 2);
            ++row_index;
            row += 8;
        } while (row_index < 4);
    }
    BattleGridOps::clear_header((BattleGridEntryView *)object);
    return 0;
}

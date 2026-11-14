/* Finds a rule by id and tells whether it applies: the first flag counts on
 * the first side, the second on the second. */
struct RuleRow {
    int id;
    int first;
    int second;
};

struct RuleTable {
    unsigned char pad_00[0x60];
    int count;
    RuleRow *rows;
};

extern "C" {

unsigned char func_089EF0A8(RuleTable *table, int id, unsigned char first, unsigned char second)
{
    if (id < 0) {
        return 0;
    }
    RuleRow *found = 0;
    for (int i = 0; i < table->count; i++) {
        RuleRow *row = &table->rows[i];
        if (row->id != id) {
            continue;
        }
        found = row;
        break;
    }
    if (found == 0) {
        return 0;
    }
    int applies = (found->first == 1 && first == 1) || (found->second == 1 && second == 1) ? 1 : 0;
    return applies;
}

}

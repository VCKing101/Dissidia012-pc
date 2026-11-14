/* Resets the whole statistics block: 19 counters, the entry table, the groups and the pairs. */
struct Table088B3224 {
    char data[0x1C20];
};

struct Group088B3224 {
    char data[0x30];
};

struct Pair088B3224 {
    char data[0x2C];
};

struct Stats088B3224 {
    int counters[19];
    Table088B3224 table;
    Group088B3224 groups[35];
    Pair088B3224 pairs[19];
    int total;
};

extern "C" {

void func_088B3004(Table088B3224 *table);
void func_088B312C(Group088B3224 *groups);
void func_088B31B4(Pair088B3224 *pairs);

void func_088B3224(Stats088B3224 *self)
{
    for (int i = 0; i < 19; i++) {
        self->counters[i] = 0;
    }
    func_088B3004(&self->table);
    func_088B312C(self->groups);
    func_088B31B4(self->pairs);
    self->total = 0;
}

}

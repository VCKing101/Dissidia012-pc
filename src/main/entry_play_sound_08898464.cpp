struct Info { int field_00; int field_04; int field_08; int field_0C; int field_10; int count; };
struct Entry { char data_00[0x22]; short sound; char data_24[0x4C - 0x24]; };
struct Table {
    char data_00[0x58]; Info *info; char data_5C[0x70 - 0x5C];
    union { Entry *entries; void *data; };
    static bool valid(Table *t, int i) {
        if (t->data != 0 && i >= 0 && i < t->info->count) {
            return true;
        }
        return false;
    }
};
extern "C" {
extern int D_089D6AC8;
void func_08893968(int player, int sound, int volume);
void func_08898464(Table *t, int i, int volume)
{
    if (Table::valid(t, i)) {
        int sound = t->entries[i].sound;
        if (sound >= 0) {
            func_08893968(D_089D6AC8, sound, volume);
        }
    }
}
}

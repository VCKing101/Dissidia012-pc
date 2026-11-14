struct Group { short ids[3]; short pad; };
struct GroupSet { int field_00; int field_04; int count; };
extern "C" {
extern GroupSet *D_089D7350;
extern Group *D_089D7354;
int func_088876B8(int unused, int id)
{
    int count = D_089D7350->count;
    for (int i = 0; i < count; i++) {
        Group *g = &D_089D7354[i];
        for (int j = 0; j < 3; j++) {
            if (g->ids[j] == id) {
                return 1;
            }
        }
    }
    return 0;
}
}

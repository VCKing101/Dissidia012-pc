typedef struct ByteMap {
    char pad_00[0x30];
    signed char values[1];
} ByteMap;

typedef struct MapUser {
    int field_00;
    ByteMap *map;
    int target;
} MapUser;

extern void func_08850FC0(int target, int value);

void func_0884ED94(MapUser *self, unsigned int idx)
{
    signed char *values = self->map->values;
    func_08850FC0(self->target, values[idx]);
}

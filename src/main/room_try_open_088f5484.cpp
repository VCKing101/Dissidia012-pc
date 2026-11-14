/* Opens room `index` when the table reports it available. */
struct Room {
    char data[0x10A8];
};

struct RoomTable {
    Room rooms[25];
};

extern "C" {
extern int func_088F5508(RoomTable *table);
extern void func_088F5188(Room *room);

bool func_088F5484(RoomTable *table, int index)
{
    if (func_088F5508(table) == 1) {
        func_088F5188(&table->rooms[index]);
        return true;
    }
    return false;
}
}

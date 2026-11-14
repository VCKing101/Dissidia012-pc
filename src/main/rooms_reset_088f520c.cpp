/* Resets the 25 0x10A8-byte rooms and the room count. */
struct Room {
    char data[0x10A8];
};

struct RoomTable {
    Room rooms[25];
    char pad_19F68[0x1A068 - 25 * 0x10A8];
    int count;
};

extern "C" {
extern void func_088F4FC8(Room *room);

void func_088F520C(RoomTable *table)
{
    for (int i = 0; i < 25; i++) {
        func_088F4FC8(&table->rooms[i]);
    }
    table->count = 0;
}
}

/* Closes the room's lobby and requests shutdown (state 3). */
#include "main/agent19_types.h"

struct NetRoom {
    int handle;
    char lobby[0x190 - 4];
    int state;
};

extern "C" {
extern void func_08903C58(void *lobby);
extern void func_0890A41C(void);
extern int func_0890314C(int handle, void (*done)(void), NetRoom *room);

bool func_08909984(NetRoom *room)
{
    Agent19Kept<NetRoom *> saved(room);
    int result;

    func_08903C58(room->lobby);
    room = saved.value;
    result = func_0890314C(room->handle, func_0890A41C, room);
    room = saved.value;
    if (result == 1) {
        room->state = 3;
        return true;
    }
    return false;
}
}

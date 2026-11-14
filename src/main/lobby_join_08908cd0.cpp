/* Joins `name` when the lobby accepts it, then moves to state 1. */
struct Lobby {
    char pad_000[0x240];
    char name[0x20];
};

extern "C" {
extern int func_0890345C(Lobby *lobby, const char *name);
extern void func_089359A4(char *dst, const char *src);
extern void func_08908FDC(Lobby *lobby, int state);

bool func_08908CD0(Lobby *lobby, const char *name)
{
    if (func_0890345C(lobby, name) == 1) {
        func_089359A4(lobby->name, name);
        func_08908FDC(lobby, 1);
        return true;
    }
    return false;
}
}

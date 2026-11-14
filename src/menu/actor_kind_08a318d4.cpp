extern "C" {
struct Actor318D4 { char pad[6]; signed char kind; };
extern int func_08A316D8(int self, int id);
extern Actor318D4 *func_088470B8(int id);
}

extern "C" int func_08A318D4(int self, int id)
{
    if (func_08A316D8(self, id) != 0) {
        return 1;
    }
    Actor318D4 *actor = func_088470B8(id);
    if (actor != 0 && actor->kind == 0x13) {
        return 1;
    }
    return 0;
}

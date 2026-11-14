/* Watch accessors guarded by func_088D86C0. */
typedef struct WatchNode {
    char pad_00[0xC];
    short kind;
} WatchNode;

typedef struct Watch {
    int field_00;
    int field_04;
    WatchNode *node;
} Watch;

extern int func_088D86C0(Watch *watch);

WatchNode *func_088D86C8(Watch *watch)
{
    if (func_088D86C0(watch) == 0) {
        return 0;
    }
    return watch->node;
}

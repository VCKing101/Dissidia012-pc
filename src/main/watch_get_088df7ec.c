/* Watch accessors guarded by func_088DF7E4. */
typedef struct WatchNode {
    char pad_00[0xC];
    short kind;
} WatchNode;

typedef struct Watch {
    int field_00;
    int field_04;
    WatchNode *node;
} Watch;

extern int func_088DF7E4(Watch *watch);

WatchNode *func_088DF7EC(Watch *watch)
{
    if (func_088DF7E4(watch) == 0) {
        return 0;
    }
    return watch->node;
}

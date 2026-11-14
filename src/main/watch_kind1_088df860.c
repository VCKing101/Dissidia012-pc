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

int func_088DF860(Watch *watch)
{
    WatchNode *node;

    if (func_088DF7E4(watch) == 0) {
        return 0;
    }
    node = watch->node;
    if (node == 0) {
        return 0;
    }
    return node->kind == 1;
}

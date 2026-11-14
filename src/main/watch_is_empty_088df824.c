typedef struct Watch {
    int field_00;
    int field_04;
    void *node;
} Watch;

extern int func_088DF7E4(Watch *watch);

int func_088DF824(Watch *watch)
{
    if (func_088DF7E4(watch) == 0) {
        return 0;
    }
    return watch->node == 0;
}

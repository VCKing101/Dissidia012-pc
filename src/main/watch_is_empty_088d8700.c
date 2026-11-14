typedef struct Watch {
    int field_00;
    int field_04;
    void *node;
} Watch;

extern int func_088D86C0(Watch *watch);

int func_088D8700(Watch *watch)
{
    if (func_088D86C0(watch) == 0) {
        return 0;
    }
    return watch->node == 0;
}

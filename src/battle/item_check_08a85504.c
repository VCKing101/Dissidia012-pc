extern int func_08B2FBCC(void *fighter, int a);
extern int func_08835778(void *fighter);
extern int func_088BF4CC(int list, int id);

/* 1 when the fighter can act and holds item 0x118 but neither 0x38 nor 0x39. */
int func_08A85504(void *fighter)
{
    int list;
    if (fighter == 0) {
        return 0;
    }
    if (func_08B2FBCC(fighter, 0) == 0) {
        return 0;
    }
    list = func_08835778(fighter);
    if (func_088BF4CC(list, 0x38) != 0) {
        return 0;
    }
    if (func_088BF4CC(list, 0x39) != 0) {
        return 0;
    }
    if (func_088BF4CC(list, 0x118) != 0) {
        return 1;
    }
    return 0;
}

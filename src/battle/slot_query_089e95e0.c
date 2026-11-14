extern int func_089E968C(unsigned int);
int func_089E95E0(unsigned int index)
{
    if (index >= 2) {
        return -1;
    }
    if (func_089E968C(index) == 1) {
        return 0;
    }
    return 1;
}

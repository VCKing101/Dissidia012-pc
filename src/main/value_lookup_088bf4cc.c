/* Looks the key up in the pair list, then in the fallback table; 0 when neither has it. */
int func_088BF52C(void *o, int key);
int func_088BD640(void *o, int key);
int func_088BF4CC(void *o, int key)
{
    int r = func_088BF52C(o, key);
    if (r != 0) {
        return r;
    }
    r = func_088BD640(o, key);
    if (r != 0) {
        return r;
    }
    return 0;
}

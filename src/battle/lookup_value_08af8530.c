/* Looks up an entry for the object's first byte and returns its mapped value, or 0. */
extern int func_088B70C8(int id);
extern int func_088C09FC(int id);

int func_08AF8530(char **self)
{
    int e = func_088B70C8(**(char **)((char *)self + 4));
    int r = 0;
    if (e != 0) {
        r = func_088C09FC(*(unsigned char *)(e + 1));
    }
    return r;
}

int func_089F0CA4(void *self, unsigned char key, char *list, int count)
{
    int i = 0;
    int found = 0;
    for (; i < count; i++) {
        if (*((unsigned char *)list + i) == key) {
            found = 1;
            break;
        }
    }
    return found;
}

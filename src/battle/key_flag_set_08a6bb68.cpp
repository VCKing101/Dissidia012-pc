/* Sets the top bit of the flag word for this key (keys from the 43-entry
 * key table; -1 bad arguments, -2 unknown key). */
extern int D_08A7AC4C[43];

extern "C" int func_08A6BB68(void *unused, unsigned int *flags, int key)
{
    if (flags == 0 || key < 0) {
        return -1;
    }
    unsigned int *found = 0;
    for (int i = 0; i < 43; i++) {
        if (D_08A7AC4C[i] != key) {
            continue;
        }
        found = &flags[i];
        break;
    }
    if (found == 0) {
        return -2;
    }
    *found = 0x80000000;
    return 0;
}

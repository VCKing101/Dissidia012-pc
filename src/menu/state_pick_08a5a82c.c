extern int func_08A5A118(unsigned char id);

int func_08A5A82C(void *self, unsigned char id, unsigned char known)
{
    if (!known) {
        if (id == 0xFF) {
            return 1;
        }
        return 0;
    }
    if (func_08A5A118(id) == 1) {
        return 3;
    }
    return 2;
}

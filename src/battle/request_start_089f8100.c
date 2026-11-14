typedef struct Request8100 {
    unsigned char active;
} Request8100;

extern int func_089F7E18(Request8100 *self);
extern int func_089F7EB8(Request8100 *self, int value, unsigned char flag);

int func_089F8100(Request8100 *self, int value, unsigned char flag)
{
    if (!self->active) {
        return -1;
    }
    if (func_089F7E18(self) != 0) {
        return -2;
    }
    if (func_089F7EB8(self, value, flag) != 0) {
        return -3;
    }
    return 0;
}

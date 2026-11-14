typedef struct MotionRequester {
    unsigned char active;
} MotionRequester;

extern int func_08A7444C(MotionRequester *self, int a, int b, int c, int d, int e, int f, int g);

int func_08A69564(MotionRequester *self)
{
    if (self->active == 0) {
        return -1;
    }
    if (func_08A7444C(self, 0, 1, 0, 0, 0, 0, 0) != 0) {
        return -2;
    }
    return 0;
}

typedef struct Range { unsigned int start; unsigned int end; } Range;
unsigned char func_0885C620(Range *self, unsigned int p)
{
    int result = 0;
    if ((unsigned int)self <= p && p < self->end) {
        result = 1;
    }
    return result;
}

typedef struct Stream { int field_00; int state; int field_08; unsigned int mode; void *buffer; unsigned char done; } Stream;
extern int func_088DF2B8(Stream *s);

int func_088DF330(Stream *s, void *buffer, unsigned int mode)
{
    if (func_088DF2B8(s) == 0) {
        return -1;
    } else if (buffer == 0 || mode > 1) {
        return -2;
    } else {
        s->field_08 = 0;
        s->mode = mode;
        s->buffer = buffer;
        s->done = 0;
        s->state = 1;
        return 0;
    }
}

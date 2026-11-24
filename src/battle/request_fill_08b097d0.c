/* Fills a four-field request record; flagged requests carry the values, others get state 4. */
extern unsigned func_089C5BC0(void);
typedef struct Request { int a; unsigned char b; int c; int state; } Request;
void func_08B097D0(Request *self, int a, unsigned char b, int c)
{
    int one;
    if ((func_089C5BC0() & 0x20) != 0) {
        self->a = a;
        self->b = b;
        self->c = c;
        one = 1;
        self->state = one;
    } else {
        one = 4;
        self->state = one;
    }
}

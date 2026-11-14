/* Resets the stream, attaches a source and starts it: -1 without a source,
   -2 when starting fails. */
typedef struct Stream0881F9C8 {
    int field_00;
    unsigned char open;
    char pad_05[3];
    void *source;
} Stream0881F9C8;

void func_0881F9AC(Stream0881F9C8 *s);
int func_0881FA30(Stream0881F9C8 *s);

int func_0881F9C8(Stream0881F9C8 *s, void *source)
{
    func_0881F9AC(s);
    if (source == 0) {
        return -1;
    }
    s->source = source;
    s->open = 1;
    if (func_0881FA30(s) != 0) {
        return -2;
    }
    return 0;
}

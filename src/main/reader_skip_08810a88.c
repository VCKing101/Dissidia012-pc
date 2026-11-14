typedef struct Reader {
    int state;
    int pos;
    int remaining;
} Reader;
extern int func_08810B04(Reader *reader);
int func_08810A88(Reader *reader, int count)
{
    int pos;
    if (func_08810B04(reader) == 0) {
        return 0;
    }
    if (count < 0) {
        return 0;
    }
    if (reader->remaining < count) {
        return 0;
    }
    pos = reader->pos;
    reader->pos += count;
    reader->remaining -= count;
    return pos;
}

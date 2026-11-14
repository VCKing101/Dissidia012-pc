typedef struct Reader {
    int state;
    void *data;
} Reader;
extern void func_088105E0(Reader *reader);
extern int func_088106AC(Reader *reader, void *data, int size);
int func_088105FC(Reader *reader, void *data, int size)
{
    func_088105E0(reader);
    if (data == 0 || size <= 0) {
        return -1;
    }
    reader->data = data;
    if (func_088106AC(reader, data, size) != 0) {
        return -3;
    }
    return 0;
}

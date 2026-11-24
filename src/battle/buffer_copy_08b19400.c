/* Resets the buffer record, then allocates and fills it from src; -1 bad arguments, -3 out of memory. */
typedef struct BufRec { unsigned char ready; char pad[3]; int size; void *data; } BufRec;
extern void func_08B193C4(BufRec *r);
extern void *func_08890900(int n);
extern void func_08935524(void *dst, void *src, int n);
int func_08B19400(BufRec *rec, void *src, int size)
{
    func_08B193C4(rec);
    if (src == 0 || size <= 0) {
        return -1;
    }
    rec->data = func_08890900(size);
    if (rec->data == 0) {
        return -3;
    }
    func_08935524(rec->data, src, size);
    rec->size = size;
    rec->ready = 1;
    return 0;
}

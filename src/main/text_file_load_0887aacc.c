typedef struct Buffer0887AACC {
    int count;
    void *data;
    int size;
    void *heap;
} Buffer0887AACC;
extern unsigned char D_089D68A8;
extern int func_088FE7C8(int file, int a);
extern int func_08862C40(int file, int a);
extern void *func_0885C6CC(void *heap, int size, int align, int tag, unsigned char one, unsigned char from_end);
extern int func_088FE708(int file, void *data, int size, int a);
extern void func_088FE944(int file, void *data, int size);
extern int func_08862854(int file, void *data, int size, int a);
extern void func_0885C538(void *heap, void *data, int zero);
extern int func_0887A898(void *data);
extern void func_0887A6A0(void *data);
int func_0887AACC(Buffer0887AACC *self_in, int file_in, void *heap_in)
{
    void *data0 = self_in->data;
    Buffer0887AACC *self = self_in;
    int file = file_in;
    void *heap = heap_in;
    int ok;
    int one = 1;

    if (data0 != 0) {
        return 0;
    }
    self->heap = heap;
    if (D_089D68A8 != 0) {
        self->size = func_088FE7C8(file, 0);
    } else {
        self->size = func_08862C40(file, 1);
    }
    if (self->size == 0) {
        return 0;
    }
    self->data = func_0885C6CC(heap, self->size, 0, 0, one, 0);
    if (self->data == 0) {
        return 0;
    }
    if (D_089D68A8 != 0) {
        ok = func_088FE708(file, self->data, self->size, 0);
        if (ok != 0) {
            func_088FE944(file, self->data, self->size);
        }
    } else {
        ok = func_08862854(file, self->data, self->size, 1);
    }
    if (ok == 0) {
        func_0885C538(heap, self->data, 0);
        self->data = 0;
        return 0;
    }
    self->count = func_0887A898(self->data);
    func_0887A6A0(self->data);
    return 1;
}

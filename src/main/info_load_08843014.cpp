/* Loads an entry's data through a temporary 64-byte block from the heap. */
struct Obj08843014 {
    void *info;
};

extern "C" {

void *func_088470B8(int id);
void *func_0885C6CC(int heap, int tag, int size, int align, int a, int b);
void func_08845230(Obj08843014 *self, int id, void *block, int tag, int mode);
void func_0885C538(int heap, void *block, int size);

void func_08843014(Obj08843014 *self, int id, int heap, int tag, int mode)
{
    self->info = func_088470B8(id);
    int one = 1;
    void *block = func_0885C6CC(heap, tag, 0x40, 0, one, 1);
    func_08845230(self, id, block, tag, mode);
    func_0885C538(heap, block, 0x40);
}

}

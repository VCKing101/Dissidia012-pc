/* Releases the attached node and frees the two buffers at 0x698/0x6A0. */
extern "C" {
extern void func_088DAB44(void *node);
extern void func_0885C538(int heap, void *block, int flags);
}

struct ViewRes {
    char pad_000[0x694];
    void *node;
    void *buffer;
    int field_69C;
    void *buffer2;

    static void detach(ViewRes *view)
    {
        if (view->node != 0) {
            func_088DAB44(view->node);
            view->node = 0;
        }
    }
};

extern "C" {
void func_088E3194(ViewRes *view, int heap)
{
    if (view->node != 0) {
        ViewRes::detach(view);
    }
    func_0885C538(heap, view->buffer, 0);
    view->buffer = 0;
    func_0885C538(heap, view->buffer2, 0);
    view->buffer2 = 0;
}
}

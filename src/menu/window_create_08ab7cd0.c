/* Allocates the 0x884-byte window on `heap` and sets up its layout.
   (Not a stack-spill function; `one` is the named constant SNC loads early.) */
typedef struct Window08AB7CD0 {
    char unknown_000[0x880];
    int f880;
} Window08AB7CD0;

typedef struct Owner08AB7CD0 {
    Window08AB7CD0 *window;
    void *heap;
} Owner08AB7CD0;

extern char D_08AB7F00[];
extern char D_08AB7F0C[];
extern char D_08AB7F18[];
void *func_0885C6CC(void *heap, unsigned size, int align, int offset, int kind, int flags);
void func_088D38F0(Window08AB7CD0 *window);
void func_088D3B5C(void *layout, int size);
void func_088D3910(int id, Window08AB7CD0 *window, void *name, int a, int b);
void func_088D3B88(Window08AB7CD0 *window, void *layout, void *name, int a, int b);
void func_088D3CF0(Window08AB7CD0 *window, void *layout, void *name);
void func_08AB7E40(Owner08AB7CD0 *owner, int page);

void func_08AB7CD0(Owner08AB7CD0 *self, void *heap)
{
    Window08AB7CD0 *window;
    int one = 1;
    self->heap = heap;
    window = (Window08AB7CD0 *)func_0885C6CC(heap, 0x884, 0, 0, one, 0);
    self->window = window;
    func_088D38F0(window);
    func_088D3B5C((char *)self->window + 0x400, 0x120);
    func_088D3910(0x1D, self->window, D_08AB7F00, 0, 1);
    func_088D3B88(self->window, (char *)self->window + 0x400, D_08AB7F0C, 0, 0x120);
    func_088D3CF0(self->window, (char *)self->window + 0x400, D_08AB7F18);
    func_08AB7E40(self, 0);
    self->window->f880 = 0;
}

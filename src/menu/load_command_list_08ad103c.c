/* Loads the command list parameter file into the buffer D_09B6CD90 allocates from a0 and records the list start and end pointers. */
typedef struct { int a; int b; } Ent;
typedef struct { char p0[0x10]; short n; short pad; Ent data[1]; } Hdr;
typedef struct { int heap; Hdr *buf; Ent *first; Ent *last; } Quad;
extern Quad D_09B6CD90;
extern const char D_08ADA518[];
extern int func_088FE7C8(const char *name, int a);
extern Hdr *func_0885C6CC(int heap, int size, int a, int b, int c, int d);
extern int func_088FE708(const char *name, Hdr *buf, int size, int a);
extern void func_088FE944(const char *name, Hdr *buf, int size);
void func_08AD103C(int heap) {
    int size;
    int h;
    int one;
    D_09B6CD90.heap = heap;
    size = func_088FE7C8(D_08ADA518, 0);
    h = D_09B6CD90.heap;
    one = 1;
    D_09B6CD90.buf = func_0885C6CC(h, size, 0, 0, one, 0);
    size = func_088FE708(D_08ADA518, D_09B6CD90.buf, size, 0);
    func_088FE944(D_08ADA518, D_09B6CD90.buf, size);
    D_09B6CD90.first = D_09B6CD90.buf->data;
    D_09B6CD90.last = D_09B6CD90.first + D_09B6CD90.buf->n;
}

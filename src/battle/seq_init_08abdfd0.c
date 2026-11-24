/* Initialises a sequence player from three source blocks, allocating one 0x74-byte slot per entry; returns 0 on success. */
typedef struct Seq { unsigned char valid; char pad0[3]; int count; char *slots; int pad1; void *src; void *b; void *c; } Seq;
typedef struct Src { char pad[0x48]; int *counts; } Src;
extern void func_08ABDF94(Seq *);
extern char *func_08ABB808(int);
extern void func_08ABC608(char *, void *);
extern int func_08ABE0F8(Seq *);
int func_08ABDFD0(Seq *self, Src *a, void *b, void *c) {
    int n;
    func_08ABDF94(self);
    if (a == 0 || b == 0 || c == 0) return -1;
    n = *a->counts;
    self->count = n;
    if (n > 0) {
        char *s = func_08ABB808(n * 0x74);
        self->slots = s;
        if (s == 0) {
            return -2;
        } else {
            int i = 0;
            int off = 0;
            if (i < self->count) {
                do {
                    func_08ABC608(self->slots + off, a);
                    i++;
                    off += 0x74;
                } while (i < self->count);
            }
        }
    }
    self->src = a;
    self->b = b;
    self->c = c;
    self->valid = 1;
    if (func_08ABE0F8(self) != 0) {
        func_08ABDF94(self);
        return -3;
    }
    return 0;
}

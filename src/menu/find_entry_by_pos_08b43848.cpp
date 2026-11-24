/* Walks the 0x100-byte entries of a list and returns the last one matching two halfword keys. */
struct Hdr { int count; short pad; short a; short b; };
extern "C" int *func_08B43848(int self_, int a, int b)
{
    int i = 0;
    Hdr *p = *(Hdr **)(self_ + 4);
    int n = p->count;
    int *r = 0;
    if (i < n) {
      do {
        if (p->a == a && p->b == b) r = (int *)p + 1;
        i++;
        p = (Hdr *)((char *)p + 0x100);
      } while (i < n);
    }
    return r;
}

/* Removes record i from an array of 0x10A8-byte records by shifting the rest down, and decrements the count (floor 0). */
struct Rec { long long d[0x215]; };
struct Pool {
    Rec recs[0x18]; int pad[0x42A]; int count;
    static int floor0(int n) { int z = 0; if (0 < n) { z = n; } return z; }
};
extern "C" void func_088F53F4(Pool *self, int i)
{
    int n = self->count - 1;
    while (i < n) {
        self->recs[i] = self->recs[i + 1];
        i++;
        n = self->count - 1;
    }
    self->count = Pool::floor0(n);
}

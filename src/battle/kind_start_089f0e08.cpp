/* Starts entry `kind` (0 or 1): -1 for a bad kind, -2 when func_08A22CA8
   returns 2 or more, otherwise passes whether it returned non-zero on.
   Retail keeps `kind` in the frame (0($sp)) across the first call: a local
   object with a constructor (Kept) is never promoted by SNC -O2, and copying
   it back right after the call puts the reload where the spill reload is. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

extern "C" {

unsigned func_08A22CA8(unsigned kind);
void func_08AFC994(unsigned kind, int flag);

int func_089F0E08(unsigned kind)
{
    if (kind >= 2) {
        return -1;
    }
    Kept<unsigned> saved(kind);
    unsigned result = func_08A22CA8(kind);
    kind = saved.value;
    if (result >= 2) {
        return -2;
    }
    func_08AFC994(kind, result != 0);
    return 0;
}

}

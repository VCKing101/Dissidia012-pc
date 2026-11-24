/* Fetches a 64-bit pair; when it is all ones asks for a fresh one instead; stores the pair in out. */
struct Pair {
    unsigned lo; unsigned hi;
    static bool Invalid(const Pair &p) { unsigned v = p.lo & p.hi; if (v == 0xFFFFFFFF) { return true; } return false; }
};
extern "C" void func_08928470(Pair *out);
extern "C" void func_08929430(Pair *out, int a);
extern "C" void func_08929A70(Pair *out, int a)
{
    Pair copy;
    Pair cur;
    Pair fresh;
    func_08928470(&cur);
    copy = cur;
    if (!Pair::Invalid(copy)) {
        *out = cur;
    } else {
        func_08929430(&fresh, a);
        *out = fresh;
    }
}

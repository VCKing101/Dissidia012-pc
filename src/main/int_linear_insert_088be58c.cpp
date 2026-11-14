/* Unguarded linear insert of a value into a sorted int range (STLport). */
struct IntLess088BE58C {
    signed char unused;
    bool operator()(const int &a, const int &b) const { return a < b; }
};
extern "C" {
void func_088BE58C(int *last, int value, IntLess088BE58C less)
{
    int *next = last;
    --next;
    while (less(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}
}

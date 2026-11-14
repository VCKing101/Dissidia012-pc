/* Value paired with the id in the owner's sorted pair list; 0 when absent. */
struct Pair088BF52C {
    int id;
    void *value;
};
struct Owner088BF52C {
    char pad_00[0x5D8];
    Pair088BF52C *pairs;
    int count;
};
struct Order088BF52C { signed char descending; Order088BF52C() : descending(0) {} };
extern "C" {
Pair088BF52C *func_088BEFE0(Pair088BF52C *begin, Pair088BF52C *end, const Pair088BF52C *key, Order088BF52C order);
void *func_088BF52C(Owner088BF52C *o, int id)
{
    Pair088BF52C *pairs = o->pairs;
    if (pairs != 0) {
        Pair088BF52C key;
        key.id = id;
        Pair088BF52C *found = func_088BEFE0(pairs, pairs + o->count, &key, Order088BF52C());
        if (found != 0) {
            return found->value;
        }
    }
    return 0;
}
}

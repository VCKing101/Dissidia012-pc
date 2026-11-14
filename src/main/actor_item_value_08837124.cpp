/* Returns the value of an actor's current item, or zero without one. The
   query record is left from an older lookup; only its result is cleared. */
struct Item08837124 {
    char pad_00[0x18];
    float value;
};

struct ItemQuery08837124 {
    int kind;
    int slot;
    int count;
    Item08837124 *found;
    int reserved[3];
};

struct Actor08837124;

extern "C" {

Item08837124 *func_08835358(Actor08837124 *self);

float func_08837124(Actor08837124 *self)
{
    ItemQuery08837124 query;
    query.found = 0;
    Item08837124 *item = func_08835358(self);
    if (item != 0) {
        return item->value;
    }
    return 0.0f;
}

}

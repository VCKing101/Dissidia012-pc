/* Opens a pair owner: both pair entries with their name tables, then the
 * binding set. */
typedef struct PairOwner {
    unsigned char open;
    unsigned char pad_01[3];
    unsigned int side;
    unsigned char first[0x34];
    unsigned char second[0x34];
    unsigned char bindings[4];
} PairOwner;

extern void func_089E39CC(PairOwner *owner);
extern int func_089E45C0(void *entry, unsigned int side, void *source, void *context, void *names, void *kinds);
extern int func_089E4010(void *bindings, void *context);
extern char D_089E3EB8[];
extern char D_089E3EC0[];
extern char D_089E3EE0[];
extern char D_089E3EE8[];

int func_089E3A0C(PairOwner *owner, unsigned int side, void *source, void *context)
{
    func_089E39CC(owner);
    if (side >= 2 || source == 0 || context == 0) {
        return -1;
    }
    if (func_089E45C0(owner->first, side, source, context, D_089E3EB8, D_089E3EC0) != 0) {
        return -2;
    }
    if (func_089E45C0(owner->second, side, source, context, D_089E3EE0, D_089E3EE8) != 0) {
        return -3;
    }
    if (func_089E4010(owner->bindings, context) != 0) {
        return -4;
    }
    owner->side = side;
    owner->open = 1;
    return 0;
}

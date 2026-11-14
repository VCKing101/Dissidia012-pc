extern "C" {
typedef struct Partner { char pad_000[0x542]; unsigned char mode; } Partner;
typedef struct Actor0883923C { char pad_000[0x304]; Partner *partner; } Actor0883923C;
extern int func_08838074(Actor0883923C *a);
extern int func_0883739C(Partner *p);
struct PartnerOps {
    static bool isThree(Partner *p) { return p->mode == 3; }
};
bool func_0883923C(Actor0883923C *a)
{
    if (func_08838074(a) == 1 && a->partner != 0 && (func_0883739C(a->partner) == 1 || PartnerOps::isThree(a->partner) == true)) {
        return true;
    }
    return false;
}
}

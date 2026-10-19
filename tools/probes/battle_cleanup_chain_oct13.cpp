extern "C" {
#include "battle/runtime_helpers.h"
}

struct BattleTraversalNodeView { BattleTraversalNodeView *field_00; };
struct BattleTraversalOwnerView {
    unsigned char padding[0x810];
    BattleTraversalNodeView *field_810;
};
struct BattleTraversalOps {
    static BattleTraversalNodeView *next(BattleTraversalOwnerView *owner, BattleTraversalNodeView *previous) {
        return previous ? previous->field_00 : owner->field_810;
    }
};
struct BattleResource18View { unsigned char padding[0x18]; void *field_18; };
struct BattleResource04View { unsigned char padding[4]; void *field_04; };

extern "C" {
void func_089EB904(void *);
void func_089EBCC8(void *);
void func_089EA680(void *);
void func_088DA060(void *);
void func_0887BA5C(void *);
void func_089E3FE4(void *);
void func_089E4594(void *);
void func_089E399C(void *);
void func_089E1248(void *);
void func_089EA74C(void *);

void func_089EA74C(void *object)
{
    BattleTraversalOwnerView *owner = (BattleTraversalOwnerView *)object;
    func_089EB904((unsigned char *)object + 0x81C);
    BattleTraversalNodeView *node = 0;
    for (;;) {
        node = BattleTraversalOps::next(owner, node);
        if (!node) break;
        func_089EBCC8(node);
    }
    func_089EA680(object);
}

void func_089E9F04(void *object)
{
    {
        int i = 0;
        unsigned char *entry = (unsigned char *)object + 0x1C;
        do {
            func_089EA74C(entry);
            ++i;
            entry += 0x84C;
        } while (i < 2);
    }
    if (((BattleResource18View *)object)->field_18) {
        func_088DA060(((BattleResource18View *)object)->field_18);
        ((BattleResource18View *)object)->field_18 = 0;
    }
    func_089E9EF4((unsigned char *)object);
}

void func_089E39CC(void *object)
{
    func_089E3FE4((unsigned char *)object + 0x70);
    func_089E4594((unsigned char *)object + 0x3C);
    func_089E4594((unsigned char *)object + 8);
    func_089E399C(object);
}

void func_089E1268(void *object)
{
    if (((BattleResource04View *)object)->field_04) {
        func_0887BA5C(((BattleResource04View *)object)->field_04);
        ((BattleResource04View *)object)->field_04 = 0;
    }
    func_089E1248(object);
}
}

extern "C" {

struct FighterCan3 {
    char pad000[0x300];
    void *partner;          /* 0x300 */
    char pad304[0xC];
    void *params;           /* 0x310 */
};

int func_088377C0(void *partner);
void *func_088BF4CC(void *params, int id);

struct FighterCan3Ops {
    static unsigned char has(FighterCan3 *fighter, int id)
    {
        return func_088BF4CC(fighter->params, id) != 0;
    }
};

unsigned char func_08B3389C(FighterCan3 *fighter)
{
    int result = 0;

    if (fighter->partner != 0 && FighterCan3Ops::has(fighter, 0x131) && func_088377C0(fighter->partner) != 0) {
        result = 1;
    }
    return result;
}

}

extern "C" {

struct FighterCan2 {
    char pad000[0x310];
    void *params;           /* 0x310 */
};

int func_088380F8(FighterCan2 *fighter);
void *func_088BF4CC(void *params, int id);

struct FighterCan2Ops {
    static unsigned char has(FighterCan2 *fighter, int id)
    {
        return func_088BF4CC(fighter->params, id) != 0;
    }
};

unsigned char func_08B2F95C(FighterCan2 *fighter)
{
    int result = 0;

    if (FighterCan2Ops::has(fighter, 0x114) && func_088380F8(fighter) != 0) {
        result = 1;
    }
    return result;
}

}

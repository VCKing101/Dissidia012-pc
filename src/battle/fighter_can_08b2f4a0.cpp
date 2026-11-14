extern "C" {

struct FighterCan {
    char pad000[0x310];
    void *params;           /* 0x310 */
    char pad314[0x88];
    void *body;             /* 0x39C */
};

int func_08B34540(FighterCan *fighter);
int func_0884E548(void *body);
void *func_088BF4CC(void *params, int id);

struct FighterCanOps {
    static unsigned char has(FighterCan *fighter, int id)
    {
        return func_088BF4CC(fighter->params, id) != 0;
    }
};

int func_08B2F4A0(FighterCan *fighter)
{
    int result = 0;

    if (func_08B34540(fighter) != 0) {
        result = 1;
    } else if (func_0884E548(fighter->body) != 0 && FighterCanOps::has(fighter, 0x119)) {
        result = 1;
    }
    return result;
}

}

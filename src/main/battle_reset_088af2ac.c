/* Resets the context's round state and reloads its saved position. */

typedef struct Ctx088AF2AC {
    char pad_00[0x814];
    int mode;
    char pad_818[0xD65 - 0x818];
    unsigned char field_D65;
    char pad_D66[0xDAC - 0xD66];
    int stage;
    char pad_DB0[0xDD4 - 0xDB0];
    int field_DD4;
    char pad_DD8[0x2B58 - 0xDD8];
    unsigned long long saved;
    char pad_2B60[0x2B80 - 0x2B60];
    unsigned char field_2B80;
} Ctx088AF2AC;

typedef struct Obj088AF2AC {
    int field_00;
    unsigned int flags;
    int field_08;
    Ctx088AF2AC *ctx;
} Obj088AF2AC;

extern unsigned long long D_089CB6F0;

int func_089A5490(unsigned long long *out, unsigned long long *in);
void func_088AF134(Obj088AF2AC *obj, int mode);

void func_088AF2AC(Obj088AF2AC *obj)
{
    unsigned long long tmp;
    obj->ctx->mode = 4;
    obj->ctx->field_DD4 = 0;
    obj->ctx->stage = 3;
    obj->ctx->field_2B80 = 0;
    obj->ctx->field_D65 = 0;
    obj->flags &= ~1;
    if (func_089A5490(&tmp, &obj->ctx->saved) < 0) {
        obj->ctx->saved = D_089CB6F0;
    }
    func_088AF134(obj, 0);
}

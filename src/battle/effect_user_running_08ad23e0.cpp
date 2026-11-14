typedef struct EffectUser {
    int pad_00;
    void *handle;
} EffectUser;

extern "C" {

extern int func_088A751C(void *handle);

/* 1 while the effect reports state 1. */
unsigned char func_08AD23E0(EffectUser *self)
{
    int result = 0;
    if (self->handle != 0 && func_088A751C(self->handle) == 1) {
        result = 1;
    }
    return result;
}

}

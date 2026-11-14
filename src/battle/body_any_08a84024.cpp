extern "C" {
extern int func_08A83C40(void *body);
extern int func_08A83C78(void *body);
extern int func_08A83CBC(void *body);
}

struct BodyOps {
    static unsigned char any(void *body)
    {
        int result = 0;
        if (func_08A83C40(body) == 1 || func_08A83C78(body) == 1 || func_08A83CBC(body) == 1) {
            result = 1;
        }
        return result;
    }
};

extern "C" {

unsigned char func_08A84024(void *body)
{
    if (body == 0) {
        return 0;
    }
    return BodyOps::any(body);
}

}

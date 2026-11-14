extern "C" {

extern int func_08A83C40(void *body);
extern int func_08A83C78(void *body);
extern int func_08A83CBC(void *body);
extern int func_08A83CF4(void *body);

bool func_08A83EB8(void *body)
{
    if (body == 0) {
        return false;
    }
    if (func_08A83C40(body) == 1) {
        return false;
    }
    if (func_08A83C78(body) == 1) {
        return false;
    }
    if (func_08A83CBC(body) == 1) {
        return false;
    }
    if (func_08A83CF4(body) == 1) {
        return false;
    }
    return true;
}

}

/* Opens a request (kind 3, mode 2, priority 3) into the result block,
 * clearing it first (-1 closed, -2 no block, -3 refused). */
typedef struct RequestResult {
    unsigned char done;
} RequestResult;

typedef struct RequestOwner {
    unsigned char open;
} RequestOwner;

extern int func_08A7444C(RequestOwner *owner, int kind, int mode, int priority, int a,
                         RequestResult *result, int b, int c);

int func_08A694E8(RequestOwner *owner, RequestResult *result)
{
    if (!owner->open) {
        return -1;
    }
    if (result == 0) {
        return -2;
    }
    result->done = 0;
    if (func_08A7444C(owner, 3, 2, 3, 0, result, 1, 0) != 0) {
        return -3;
    }
    return 0;
}

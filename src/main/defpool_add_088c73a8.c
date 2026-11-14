/* Takes the next definition set of the pool for a new id; 0 when no id is free. */
typedef struct DefSet088C73A8 { short id; char data[0x136]; } DefSet088C73A8;
typedef struct DefPool088C73A8 { DefSet088C73A8 sets[20]; int count; } DefPool088C73A8;
int func_088C7600(DefPool088C73A8 *pool);
void func_088C7274(DefSet088C73A8 *set, int id, int a, int b, int c);
DefSet088C73A8 *func_088C73A8(DefPool088C73A8 *pool, int a, int b, int c)
{
    DefSet088C73A8 *result = 0;
    int id = func_088C7600(pool);
    if (id != -1) {
        result = &pool->sets[pool->count];
        func_088C7274(result, id, a, b, c);
        pool->count++;
    }
    return result;
}

/* Definition set of the pool with the id; 0 when none. */
struct DefSet088C75B4 { short id; char data[0x136]; };
struct DefPool088C75B4 { DefSet088C75B4 sets[20]; int count; };
extern "C" {
DefSet088C75B4 *func_088C75B4(DefPool088C75B4 *pool, int id)
{
    DefSet088C75B4 *result = 0;
    for (int i = 0; i < pool->count; i++) {
        if (pool->sets[i].id == id) {
            result = &pool->sets[i];
            break;
        }
    }
    return result;
}
}

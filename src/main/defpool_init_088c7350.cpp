/* Initialises all twenty definition sets of the pool and empties it. */
struct DefSet088C7350 { short id; char data[0x136]; };
struct DefPool088C7350 { DefSet088C7350 sets[20]; int count; };
extern "C" {
void func_088C7218(DefSet088C7350 *set);
void func_088C7350(DefPool088C7350 *pool)
{
    for (int i = 0; i < 20; i++) {
        func_088C7218(&pool->sets[i]);
    }
    pool->count = 0;
}
}

/* Looks the key up in the table and records whether it was found. */
struct Big088C39D8 { char pad[0xE788]; int table; };
struct Result088C39D8 { int ok; int done; };
extern "C" {
int func_088B90F8(int *table, int key);
bool func_088C39D8(Result088C39D8 *r, Big088C39D8 *big, int *key)
{
    r->ok = (unsigned char)(func_088B90F8(&big->table, *key) != 0);
    r->done = 1;
    return r->ok != 0;
}
}

/* Looks the key up in the table and records whether it was found. */
struct Big088C38A0 { char pad[0xE788]; int table; };
struct Result088C38A0 { int ok; int done; };
extern "C" {
int func_088B90C4(int *table, int key);
bool func_088C38A0(Result088C38A0 *r, Big088C38A0 *big, int *key)
{
    r->ok = (unsigned char)(func_088B90C4(&big->table, *key) != 0);
    r->done = 1;
    return r->ok != 0;
}
}

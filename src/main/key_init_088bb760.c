/* Initialises a search key with an id and returns it. */
typedef struct Key088BB760 {
    unsigned short id;
} Key088BB760;

Key088BB760 *func_088BB760(Key088BB760 *key, int id)
{
    key->id = id;
    return key;
}

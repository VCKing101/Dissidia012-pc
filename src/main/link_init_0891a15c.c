extern void func_0892A13C(void *list);

typedef struct LinkInfo {
    short id;
    short count;
    int owner;
    short prev;
    short next;
    short parent;
    short depth;
    short child;
    short flags;
    char list[4];               /* 0x14 */
} LinkInfo;

void func_0891A15C(LinkInfo *link)
{
    link->id = -1;
    link->count = 0;
    link->owner = -1;
    link->prev = -1;
    link->next = -1;
    link->parent = -1;
    link->depth = 0;
    link->child = -1;
    link->flags = 0;
    func_0892A13C(link->list);
}

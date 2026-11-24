/* Walks the list and sets flag bit 2 on nodes whose func_08B4AB10 value is 2. */
typedef struct Node { int pad0; int key; char pad[0x20]; unsigned flags; struct Node *next; } Node;
typedef struct List { Node *head; } List;
extern int func_08B4AC70(int arg);
extern int func_08B4AB10(int h, int key);

void func_08AE9FEC(List *list)
{
    int h = func_08B4AC70(0x2000);
    if (h != 0) {
        Node *n = 0;
        while ((n = n == 0 ? list->head : n->next) != 0) {
            int v;
            v = func_08B4AB10(h, n->key);
            if (v == 2) {
                n->flags = n->flags | 2;
            }
        }
    }
}

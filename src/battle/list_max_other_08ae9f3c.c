/* Walks the list and returns the largest func_08B4AB10 value over nodes other than skip. */
typedef struct Node { int pad0; int key; char pad[0x24]; struct Node *next; } Node;
typedef struct List { Node *head; } List;
extern int func_08B4AC70(int arg, int key);
extern int func_08B4AB10(int h, int key);

int func_08AE9F3C(List *list, Node *skip)
{
    int h = func_08B4AC70(0x2000, skip->key);
    int best = 0;
    if (h != 0) {
        Node *n = 0;
        Node *c;
        while ((n = c = n == 0 ? list->head : n->next) != 0) {
            if (c != skip) {
                int v = func_08B4AB10(h, n->key);
                if (best < v) {
                    best = v;
                }
            }
        }
    }
    return best;
}

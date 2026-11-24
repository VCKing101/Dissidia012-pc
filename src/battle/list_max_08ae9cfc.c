/* Walks the list and returns the largest value func_08B4AB10 gives for the nodes' key. */
typedef struct Node { int pad0; int key; char pad[0x24]; struct Node *next; } Node;
typedef struct List { Node *head; } List;
extern int func_08B4AC70(int arg);
extern int func_08B4AB10(int h, int key);

int func_08AE9CFC(List *list)
{
    int h = func_08B4AC70(0x2000);
    int best = 0;
    if (h != 0) {
        Node *n = 0;
        while ((n = n == 0 ? list->head : n->next) != 0) {
            int v;
            v = func_08B4AB10(h, n->key);
            if (best < v) {
                best = v;
            }
        }
    }
    return best;
}

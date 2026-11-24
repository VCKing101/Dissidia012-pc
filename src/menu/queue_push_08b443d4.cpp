/* Appends a node to the tail of the queue at D_08B44F08 + 0xC (head at +0, tail at +4). */
struct Node { Node *next; };
struct Queue { Node *head; Node *tail; };
extern "C" char *D_08B44F08;
struct Pusher {
    static void run(Node *node)
    {
        Queue *q = (Queue *)(D_08B44F08 + 0xC);
        Node *tail = q->tail;
        if (tail == 0) {
            q->tail = node;
            q->head = node;
            node->next = 0;
        } else {
            tail->next = node;
            node->next = 0;
            q->tail = node;
        }
    }
};
extern "C" void func_08B443D4(Node *node)
{
    Pusher::run(node);
}

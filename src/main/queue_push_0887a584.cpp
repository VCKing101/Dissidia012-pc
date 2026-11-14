/* Intrusive queue: nodes carry their link at 0x10. */
struct QNode;

struct QLink {
    QNode *next;
    QNode *prev;
};

struct QNode {
    char pad_00[0x10];
    QLink link;

    static QLink *linkOf(QNode *n) { return &n->link; }
};

struct Queue {
    char pad_00[0x1C];
    QNode *head;
    QNode *tail;
    int count;
};

extern "C" {

void func_0887A584(Queue *q, QNode *n)
{
    if (q->tail == 0) {
        q->tail = n;
        q->head = n;
        n->link.next = 0;
        n->link.prev = 0;
    } else {
        QNode::linkOf(q->tail)->next = n;
        QNode *prev = q->tail;
        n->link.next = 0;
        n->link.prev = prev;
        q->tail = n;
    }
    q->count++;
}

}

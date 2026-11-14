/* Queues an effect request (two slots): 1 when queued, 0 when full. */
typedef struct Request08853E24 {
    int id;
    short value;
    unsigned char kind;
    unsigned char mode;
    int field_08;
    float scale;
    float a;
    float b;
} Request08853E24;

typedef struct Queue08853E24 {
    Request08853E24 requests[2];
    int count;
} Queue08853E24;

int func_08853E24(Queue08853E24 *q, int id, int value, int mode, int kind, float a, float b)
{
    int n = q->count;
    Request08853E24 *r;
    if (n >= 2) {
        return 0;
    }
    r = &q->requests[n];
    r->id = id;
    r->value = value;
    r->scale = 1.0f;
    r->mode = mode;
    r->kind = kind;
    r->a = a;
    r->b = b;
    q->count = n + 1;
    return 1;
}

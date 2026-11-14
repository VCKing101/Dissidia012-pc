/* Effect manager: clear and shutdown. */
struct Node0881B4A0 {
    Node0881B4A0 *next;
};

struct List0881B4A0 {
    Node0881B4A0 *first;
    Node0881B4A0 *last;

    static void clear(List0881B4A0 *l)
    {
        l->last = 0;
        l->first = 0;
    }

    static bool empty(List0881B4A0 *l) { return l->first == 0; }

    static Node0881B4A0 *pop(List0881B4A0 *l)
    {
        Node0881B4A0 *n = l->first;
        if (n != 0 && l->last != 0) {
            l->first = n->next;
            if (n == l->last) {
                Node0881B4A0 *none = 0;
                l->last = none;
            }
            n->next = 0;
        }
        return n;
    }
};

struct EffectMgr0881B4A0 {
    unsigned char ready;
    char pad_01[3];
    int frame;
    unsigned char locked;
    char pad_09[7];
    unsigned char paused;
    char pad_11[3];
    char sounds[0x20];
    char lights[0x28];
    char nodes[0x14];
    int a;
    int b;
    char pad_78[8];
    float pos[4];               /* 0x80 */
    float fade;                 /* 0x90 */
    char pad_94[0xC];
    float vecs[7][4];           /* 0xA0 */
    List0881B4A0 lists[6];      /* 0x110 */
    void *buffer_a;             /* 0x140 */
    void *buffer_b;             /* 0x144 */
};

extern "C" {

void func_089A3890(float *v);
void func_08842138(void *buffer);
void func_08818778(Node0881B4A0 *node, int flags);
void func_08819C44(char *nodes);
void func_0881A284(char *lights);
void func_088167F0(char *sounds);

void func_0881B4A0(EffectMgr0881B4A0 *self)
{
    self->ready = 0;
    self->frame = 0;
    self->locked = 0;
    self->paused = 0;
    self->a = 0;
    self->b = 0;
    func_089A3890(self->pos);
    self->fade = 0.0f;
    func_089A3890(self->vecs[0]);
    func_089A3890(self->vecs[1]);
    func_089A3890(self->vecs[2]);
    func_089A3890(self->vecs[3]);
    func_089A3890(self->vecs[4]);
    func_089A3890(self->vecs[5]);
    func_089A3890(self->vecs[6]);
    List0881B4A0::clear(&self->lists[0]);
    List0881B4A0::clear(&self->lists[1]);
    List0881B4A0::clear(&self->lists[2]);
    List0881B4A0::clear(&self->lists[3]);
    List0881B4A0::clear(&self->lists[4]);
    List0881B4A0::clear(&self->lists[5]);
    self->buffer_a = 0;
    self->buffer_b = 0;
}

void func_0881B558(EffectMgr0881B4A0 *self)
{
    if (self->buffer_b != 0) {
        func_08842138(self->buffer_b);
    }
    if (self->buffer_a != 0) {
        func_08842138(self->buffer_a);
    }
    while (!List0881B4A0::empty(&self->lists[0])) {
        func_08818778(List0881B4A0::pop(&self->lists[0]), 3);
    }
    func_08819C44(self->nodes);
    func_0881A284(self->lights);
    func_088167F0(self->sounds);
    func_0881B4A0(self);
}

}

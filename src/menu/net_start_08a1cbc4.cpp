#include "menu/agent29_types.h"

/* MenuRoot: starts the network page: resets the root's network state,
   and unless the request is a return (state 3) makes the network object
   in a 0x31970-byte block of the heap, adds the root's node (+0x16C) to
   the list D_089D8360, then starts the object and the session. */
typedef struct NetNode29 {
    NetNode29 *next;
} NetNode29;

typedef struct NetList29 {
    char pad_000[0x194];
    NetNode29 *head;
    NetNode29 *tail;

    static void Add(NetList29 *list, NetNode29 *node) {
        if (list->tail == 0) {
            list->tail = node;
            list->head = node;
            node->next = 0;
        } else {
            list->tail->next = node;
            node->next = 0;
            list->tail = node;
        }
    }
} NetList29;

typedef struct NetRequest29 {
    int state;
} NetRequest29;

extern "C" {
extern NetList29 *D_089D8360;
extern NetRequest29 D_09B6D840;
void func_08A1CB58(struct NetRoot29 *self);
void func_088F7834(void *net, void *config);
void func_088F78B0(void *net, void *heap);
void func_088F6798(void);
void func_088F707C(void *session);
}

/* The root with the network fields that follow its region header part. */
typedef struct NetRoot29 {
    MenuRoot root;
    char pad_158[0x4];
    char config[0x8];  /* +0x15C */
    char session[0x8]; /* +0x164 */
    NetNode29 node;    /* +0x16C */
} NetRoot29;

struct Net29 {
    char data[0x31970];
    static void *operator new(unsigned size, void *place) { return place; }
    Net29(void *config) { func_088F7834(this, config); }
};

extern "C" void func_08A1CBC4(NetRoot29 *self)
{
    WORK_INT(self->root.work, 0x8E60) = 0;
    WORK_BYTE(self->root.work, 0x8E64) = 0;
    WORK_INT(self->root.work, 0x696C) = -1;
    WORK_INT(self->root.work, 0x25B80) = 0;
    WORK_INT(self->root.work, 0x25B84) = -1;
    WORK_INT(self->root.work, 0x6968) = 0;
    func_08A1CB58(self);
    if (D_09B6D840.state == 3) {
        return;
    }
    WORK_INT(self->root.work, 0x6960) = (int)Heap29::Alloc(self->root.heap, 0x31970, true);
    WORK_INT(self->root.work, 0x6964) = (int)new ((void *)WORK_INT(self->root.work, 0x6960)) Net29(self->config);
    NetList29 *list = D_089D8360;
    if (list != 0) {
        NetList29::Add(list, &self->node);
    }
    func_088F78B0((void *)WORK_INT(self->root.work, 0x6964), self->root.heap);
    func_088F6798();
    func_088F707C(self->session);
}

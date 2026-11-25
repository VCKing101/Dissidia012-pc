/* Each frame: bind a screen's texts to the layout nodes listed in a binding table (0xFFFF id ends it). For each
   entry whose text slot is set, 0x088D4584 copies the node's matrix and colour into that text. See
   docs/decomp/text-system.md, section 4 (layout nodes). */
typedef unsigned short u16;

typedef struct NodeTextBinding {
    u16 slot; /* index into the screen's text table */
    u16 pad;
    u16 id;   /* layout node id; 0xFFFF ends the table */
    u16 arg;
} NodeTextBinding;

extern void func_088D4584(void *layout, int id, int arg, void *text);

void func_088D44EC(void *layout, int *texts, NodeTextBinding *bindings)
{
    NodeTextBinding *p = bindings;
    int end = 0xFFFF;
    for (;;) {
        u16 id = p->id;
        int text;
        if (id == end) break;
        text = texts[p->slot];
        if (text != 0) {
            func_088D4584(layout, id, p->arg, (void *)text);
        } else {
        }
        p++;
    }
}

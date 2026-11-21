typedef struct Step08884BC0 {
    unsigned short unk_00;
    unsigned short state;              /* +0x02 */
    unsigned short next;               /* +0x04 */
    unsigned short unk_06[2];
} Step08884BC0;

typedef struct Child08884BC0 {
    unsigned char unk_00[0xC];
    short state;                       /* +0x0C */
} Child08884BC0;

typedef struct Holder08884BC0 {
    int unk_00;
    Child08884BC0 *child;              /* +0x04 */
    int unk_08;
    void *text0;                       /* +0x0C */
    void *text1;                       /* +0x10 */
    int step;                          /* +0x14 index into D_089C9F88 */
} Holder08884BC0;

extern Step08884BC0 D_089C9F88[];
extern void func_088D9F5C(Child08884BC0 *child, unsigned short state, int unk);
extern void func_088DA060(Child08884BC0 *child);
extern void func_0887BA5C(void *text);

void func_08884BC0(Holder08884BC0 *self)
{
    Child08884BC0 *child = self->child;

    if (child != 0) {
        if (child->state == D_089C9F88[self->step].state) {
            func_088D9F5C(child, D_089C9F88[self->step].next, 1);
        } else {
            if (child != 0) {
                func_088DA060(child);
                self->child = 0;
            }
            if (self->text0 != 0) {
                func_0887BA5C(self->text0);
                self->text0 = 0;
            }
            if (self->text1 != 0) {
                func_0887BA5C(self->text1);
                self->text1 = 0;
            }
        }
    }
}

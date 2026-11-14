/* Flushes the queue and sends event 1 with `flag`, unless func_08B0DF10 says not to. */
struct Owner08B062C8;

extern void *D_08B01CF8;
extern int func_08B0DF10(struct Owner08B062C8 *self);
extern void func_08AF3ECC(void *queue);
extern void func_08AFD194(void *queue, int event, unsigned char flag);

void func_08B062C8(struct Owner08B062C8 *self, unsigned char flag)
{
    if (func_08B0DF10(self) != 0) {
        return;
    }
    func_08AF3ECC(D_08B01CF8);
    func_08AFD194(D_08B01CF8, 1, flag);
}

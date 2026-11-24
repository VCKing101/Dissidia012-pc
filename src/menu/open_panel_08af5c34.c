/* Refreshes a panel record: updates its widgets from the record's stored ids and sets the panel frame. */
extern void func_08AF5EC8(void *owner, int *rec);
extern void func_088D46E4(void *a, int b, int c, int d, int e, int f);
extern void func_088D3F7C(void *a, int b, int c, int d);
extern void func_088D3B88(int a, int b, int c, int d, int e);
extern void func_088D3CF0(int a, int b, int c);
extern void func_088D428C(int a, int b, int c, int d);

typedef struct O5C34 { char pad[0x140]; struct { char pad[0x89C]; void *h; } *objs; } O5C34;
typedef struct W5C34 { char pad[0xC]; short idle; short frame; } W5C34;
typedef struct Q5C34 { char pad[0x5C]; W5C34 *w; } Q5C34;

void func_08AF5C34(O5C34 *owner, int *rec, int frame)
{
    if (((Q5C34 *)rec[4])->w != 0 && ((Q5C34 *)rec[4])->w->idle != -1) {
        func_08AF5EC8(owner, rec);
    }
    func_088D46E4(rec + 6, rec[0], rec[1], rec[2], 0, -1);
    func_088D3F7C(owner->objs->h, rec[5], rec[0x13], rec[0x16]);
    func_088D3B88(rec[3], rec[4], rec[0x11], 0, 0x120);
    func_088D3CF0(rec[3], rec[4], rec[0x12]);
    func_088D3CF0(rec[3], rec[4], rec[0x14]);
    ((Q5C34 *)rec[4])->w->frame = frame;
    func_088D428C(rec[4], rec[5], rec[0x15], 1);
}

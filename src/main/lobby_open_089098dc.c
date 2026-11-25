/* Opens the LOBBY<n> file for the lobby; on success copies 0x24B 8-byte records from src into +0x198 and marks it loaded. */
extern int func_0893476C(char *buf, const char *fmt, int n);
extern int func_08902E3C(int h, char *name, void (*cb)(void), void *self);
extern void func_0890A400(void);
extern const char D_089CDD30[];
typedef struct Rec { int a, b; } Rec;
typedef struct S { int h; char p[0x18C]; int loaded; int pad; Rec recs[0x24B]; } S;
int func_089098DC(S *self, int n, Rec *src) {
    char name[32];
    int i;
    int cnt;
    int r;
    int yes;
    func_0893476C(name, D_089CDD30, n);
    r = func_08902E3C(self->h, name, func_0890A400, self);
    yes = 1;
    if (yes == r) {
        char *db = (char *)self->recs;
        char *bs = (char *)src;
        i = 0;
        cnt = 0x24B;
        do {
            Rec *s = (Rec *)(bs + i);
            int w0 = s->a;
            int w1 = s->b;
            Rec *d = (Rec *)(db + i);
            d->a = w0;
            d->b = w1;
            cnt--;
            i += 8;
        } while (cnt != 0);
        self->loaded = yes;
        return 1;
    }
    return 0;
}

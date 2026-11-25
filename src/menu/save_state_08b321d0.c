/* With a non-zero mode, saves the mode byte and a 0x12C-byte copy of the block's state into D_09B6DD10 and appends its word +0x18 to the list from func_08AF34A0; with zero clears the saved mode, runs func_08B32164 and empties the list; then hands the list on. */
typedef struct State08B321D0 { int w[0x4B]; } State08B321D0;
typedef struct Blk08B321D0 { char pad[0x40C]; int w40C; char pad1[0x444 - 0x410]; State08B321D0 st; } Blk08B321D0;
typedef struct Saved08B321D0 { int w0; unsigned char b4; char pad[3]; State08B321D0 st; } Saved08B321D0;
typedef struct S08B321D0 { char pad[0x138]; Blk08B321D0 *blk; } S08B321D0;
extern Saved08B321D0 D_09B6DD10;
extern void *D_08B01CF8;
extern void func_08B2BB2C(int w, State08B321D0 *st);
extern int *func_08AF34A0(void *g);
extern void func_08B32164(S08B321D0 *self);
extern void func_08AF34B0(void *g, int *list);
void func_08B321D0(S08B321D0 *self, unsigned char mode)
{
    int *list;
    func_08B2BB2C(self->blk->w40C, &self->blk->st);
    list = func_08AF34A0(D_08B01CF8);
    if (mode != 0) {
        D_09B6DD10.b4 = mode;
        D_09B6DD10.st = self->blk->st;
        list[1 + list[0]] = D_09B6DD10.st.w[4];
    } else {
        *((unsigned char *)&D_09B6DD10 + 8) = 0;
        func_08B32164(self);
        list[0] = 0;
    }
    func_08AF34B0(D_08B01CF8, list);
}

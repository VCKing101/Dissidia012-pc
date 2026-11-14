/* Opens the list scroller on the current page's range and registers the page frames. */
typedef struct OpenScroller {
    unsigned int field_00;
} OpenScroller;

typedef struct OpenListState {
    char pad_0000[0x400];
    char field_400[0x25BC - 0x400];
    OpenScroller field_25BC;
    char pad_25C0[0x2698 - 0x25C0];
    int field_2698;
    char pad_269C[0x4];
    int field_26A0[4];
} OpenListState;

typedef struct OpenListOwner {
    char pad_0000[0xA690];
    OpenListState *field_A690;
} OpenListOwner;

extern char D_08AEB6B8[];
extern void func_088D46E4(OpenScroller *scroller, int first, int zero, int last, int none, int mode);
extern void func_088D3CF0(void *state, void *frames, void *table);

void func_08AE5644(OpenListOwner *self)
{
    OpenListState *state = self->field_A690;
    OpenScroller *scroller = &state->field_25BC;
    func_088D46E4(scroller, 0, 0, state->field_26A0[state->field_2698] - 1, 0, 9);
    scroller = &self->field_A690->field_25BC;
    scroller->field_00 = scroller->field_00 | 4;
    func_088D3CF0(self->field_A690, self->field_A690->field_400, D_08AEB6B8);
}

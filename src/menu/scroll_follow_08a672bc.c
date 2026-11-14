/* Scrolls the list one row towards the cursor when it leaves the visible window. */
typedef struct FollowList {
    char pad_00[0x50];
    int field_50;
    char pad_54[0x7C - 0x54];
    int field_7C;
} FollowList;

extern void func_08A67540(FollowList *self);
extern void func_08A67588(FollowList *self);
extern void func_08A67420(FollowList *self);

void func_08A672BC(FollowList *self)
{
    int cursor = self->field_50 - 2;
    int scroll = self->field_7C;
    int previous = scroll;
    if (cursor >= 0 && scroll > cursor) {
        scroll = scroll - 1;
        self->field_7C = scroll;
    } else if (scroll + 6 <= cursor) {
        scroll = scroll + 1;
        self->field_7C = scroll;
    }
    if (previous != scroll) {
        func_08A67540(self);
        func_08A67588(self);
        func_08A67420(self);
    }
}

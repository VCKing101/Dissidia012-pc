/* Updates the result screen when it exists. */
typedef struct ResultOwner {
    char pad_00[0x30];
    void *screen;
} ResultOwner;

extern void func_088FC588(ResultOwner *owner);

void func_088FCAB8(ResultOwner *owner)
{
    if (owner->screen == 0) {
        return;
    }
    func_088FC588(owner);
}

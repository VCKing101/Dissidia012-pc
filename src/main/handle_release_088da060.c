/* Releases the object at 0x24 through its owner and clears it. */
typedef struct HandleOwner {
    char pad_00[0x24];
    void *object;
} HandleOwner;

extern void func_088DAAA0(void *object, HandleOwner *owner);

void func_088DA060(HandleOwner *owner)
{
    if (owner->object == 0) {
        return;
    }
    func_088DAAA0(owner->object, owner);
    owner->object = 0;
}

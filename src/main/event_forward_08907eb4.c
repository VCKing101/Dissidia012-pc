/* Forwards an event to the owner's target with the owner's context as the fifth argument. */
typedef struct EventOwner {
    int field_00;
    void *target;
    void *context;
} EventOwner;

extern void func_08907D54(void *target, int a, int b, int c, void *context);

void func_08907EB4(EventOwner *owner, int a, int b, int c)
{
    func_08907D54(owner->target, a, b, c, owner->context);
}

/* Deletes a child object of the pair display. */
class DisplayObject {
public:
    virtual ~DisplayObject();
};

extern "C" int func_08A02E38(void *display, DisplayObject *child)
{
    if (child == 0) {
        return -1;
    }
    delete child;
    return 0;
}

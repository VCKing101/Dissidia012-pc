typedef struct StreamCursor {
    char pad_00[0x6C];
    int count;
    int field_70;
    int playing;
    int track;
} StreamCursor;

int func_088EB07C(StreamCursor *cursor)
{
    int next = cursor->track + 1;

    if (next >= cursor->count) {
        return 0;
    }
    return next;
}

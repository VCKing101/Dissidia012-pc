/* Next track index after the one at 0x74, wrapping at the count at 0x6C. */
typedef struct StreamCursor {
    char pad_00[0x6C];
    int count;
    int field_70;
    int playing;
    int track;
} StreamCursor;

int func_088EB054(StreamCursor *cursor)
{
    int next = cursor->playing + 1;

    if (next >= cursor->count) {
        return 0;
    }
    return next;
}

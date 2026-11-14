typedef struct Link { unsigned short id; short field_02; unsigned short target; unsigned short mode; } Link;
extern void func_088D3AE8(void *ctx, unsigned short target, unsigned short mode, unsigned short id);

void func_088D3F00(void *owner, void *ctx, Link *links)
{
    Link *cursor = links;
    for (;;) {
        if (cursor->id == 0xFFFF) {
            break;
        }
        if (cursor->target != 0xFFFF) {
            func_088D3AE8(ctx, cursor->target, cursor->mode, cursor->id);
        }
        cursor++;
    }
}

/* Image library: take the image, palette and sequence chunks of a GIM picture. */
#include "main/sdk_gcc1_types.h"

int func_0899EA2C(SdkGimChunk *chunk, int id);
void func_08935524(void *dst, const void *src, unsigned int size);
void func_089C6040(void);

int func_0899BC64(SdkImage *img, SdkGimChunk *picture)
{
    SdkGimChunk *chunk;
    SdkGimChunk *end;
    SdkGimImage *copy;
    char *src;
    unsigned int size;
    unsigned int *offsets;
    int n;

    if (img == 0)
        return 0;
    if (picture == 0)
        return 0;
    func_0899BEE0(img);
    n = func_0899EA2C(picture, 6);
    if (n > 0) {
        img->sequences = func_0899B75C(n * 4);
        if (img->sequences == 0)
            return 0;
    }
    end = (SdkGimChunk *)((char *)picture + picture->next);
    for (chunk = (SdkGimChunk *)((char *)picture + picture->child); chunk < end;
         chunk = (SdkGimChunk *)((char *)chunk + chunk->next)) {
        size = chunk->child - chunk->data;
        src = (char *)chunk + chunk->data;
        switch (chunk->id) {
        case 4:
            if (img->image != 0)
                break;
            copy = func_0899B794(size);
            if (copy == 0)
                return 0;
            img->image = copy;
            func_08935524(copy, src, size);
            copy->refs = 1;
            offsets = (unsigned int *)((char *)copy + copy->index_start);
            for (n = copy->level_count * copy->frame_count; n > 0; n--) {
                unsigned int *p = offsets++;
                *p += (unsigned int)copy;
            }
            if (copy->width > 512 || copy->height > 512)
                return 0;
            break;
        case 5:
            if (img->palette != 0)
                break;
            copy = func_0899B794(size);
            if (copy == 0)
                return 0;
            img->palette = copy;
            func_08935524(copy, src, size);
            copy->refs = 1;
            offsets = (unsigned int *)((char *)copy + copy->index_start);
            for (n = copy->level_count * copy->frame_count; n > 0; n--) {
                unsigned int *p = offsets++;
                *p += (unsigned int)copy;
            }
            if (copy->width > 256)
                return 0;
            break;
        case 6:
            copy = func_0899B75C(size);
            if (copy == 0)
                return 0;
            img->sequences[img->num_sequences++] = (SdkGimSequence *)copy;
            func_08935524(copy, src, size);
            copy->refs = 1;
            break;
        }
    }
    func_089C6040();
    func_0899BFE0(img);
    return 1;
}

#include "main/auto_region1.h"

/* Clears the actor's id bit in both bit sets. */
void func_0880AE5C(ActorBits *bits, Actor *actor)
{
    unsigned int *word;

    word = &bits->bits_04[(unsigned int)(int)actor->id >> 5];
    *word = *word & ~(1 << ((int)actor->id & 0x1fU));
    word = &bits->bits_24[(unsigned int)(int)actor->id >> 5];
    *word = *word & ~(1 << ((int)actor->id & 0x1fU));
}

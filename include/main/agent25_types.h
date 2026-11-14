/* Shared structures for agent25's main-module matches. */
#ifndef AGENT25_TYPES_H
#define AGENT25_TYPES_H

#ifdef __cplusplus
/* A value kept in the frame across calls (see build/dc-results/stack-spill.md). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};
#endif

#endif

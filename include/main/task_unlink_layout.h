#ifndef MAIN_TASK_UNLINK_LAYOUT_H
#define MAIN_TASK_UNLINK_LAYOUT_H
#include "main/task_runtime.h"

/* The target uses flat integer addresses. Use an address-width integer and
 * the observed contiguous suffix rather than narrowing host pointers or
 * evaluating a member through a null pointer. Native/host layout checks pin
 * these offsets to the actual fields before this view is used. */
typedef __typeof__(sizeof(0)) TaskQueueAddress;
typedef char TaskQueueAddressWidthCheck[(sizeof(TaskQueueAddress) == sizeof(MainTask *)) ? 1 : -1];
struct TaskQueueLinkSuffix {
    MainTask *following;
    MainTask *previous;
    MainTask *auxiliary_following;
    MainTask *auxiliary_previous;
    unsigned int first;
    unsigned int second;
};
#endif

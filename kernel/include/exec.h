#pragma once
#if __KERNEL__
#include "lib/compiler_attributes.h"

__nonnull(1, 2) int execve(char *, char **, char **);
#endif

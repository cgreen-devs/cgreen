/*
  This file used to be a link to the corresponding .c file because we
  want to compile the same tests for C and C++. But since some systems
  don't handle symbolic links the same way as *ix systems we get
  inconsistencies (looking at you Cygwin) or plain out wrong (looking
  at you MSYS2, copying ?!?!?) behaviour.

  So we will simply include the complete .c source instead...
 */

#include "double_tests.c"

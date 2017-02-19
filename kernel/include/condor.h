#include <stddef.h>

#ifndef CONDOR_KERNEL_H
#define CONDOR_KERNEL_H

//Macros

//Kernel version
#define KENREL_TYPE_ALPHA 0
#define KENREL_TYPE_BETA 1
#define KENREL_TYPE_RC 2
#define KENREL_TYPE_RELEASE 3

#define KENREL_MAJOR 0
#define KENREL_MINOR 1
#define KENREL_PATCH 1
#define KERNEL_TYPE  KENREL_TYPE_ALPHA

//Typedefs

//Unsigned types
typedef unsigned long long uqword_t;
typedef unsigned long udword_t;
typedef unsigned uword_t;
typedef unsigned char ubyte_t;
typedef unsigned char uchar_t;

//Signed types
typedef   signed long long sqword_t;
typedef   signed long sdword_t;
typedef   signed sword_t;
typedef   signed char sbyte_t;
typedef   signed char schar_t;

typedef   signed long long qword_t;
typedef   signed long dword_t;
typedef   signed word_t;
typedef   signed char byte_t;
typedef   signed char char_t;

extern uqword_t KERNEL_VIRTUAL_BASE;

/* Utilities */
void kexit(int status);
void kpanic(const char* message);
void kputchar(const char c);
void kdump_useStack(uqword_t* esp);
void kdump_useRegs(uqword_t eip);
void kdumpStack(uqword_t* esp, udword_t ebp);

//Optimized memory functions
/*
void* kmemcpy(void* dest, const void* src, size_t num);
void* kmemmove(void* dest, const void* src, size_t num);
*/

#endif

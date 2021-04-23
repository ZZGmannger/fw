#ifndef GSI_DEF_H
#define GSI_DEF_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "export.h"
 

typedef signed   char                   s_int8_t;      /**<  8bit integer type */

typedef signed   short                  s_int16_t;     /**< 16bit integer type */
typedef signed   int                    s_int32_t;     /**< 32bit integer type */
typedef unsigned char                   s_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  s_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned int                    s_uint32_t;    /**< 32bit unsigned integer type */

typedef signed long long                s_int64_t;     /**< 64bit integer type */
typedef unsigned long long              s_uint64_t;    /**< 64bit unsigned integer type */


typedef int                             s_bool_t;      /**< boolean type */
typedef long                            s_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   s_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef s_base_t                       	s_err_t;       /**< Type for error number */
typedef s_ubase_t                      	s_size_t;      /**< Type for size number */


/* boolean type definitions */
#define GSI_TRUE                         	1               /**< boolean true  */
#define GSI_FALSE                        	0               /**< boolean fails */
#define GSI_NULL                            0              
 
	  
/*General systerm Interface status*/
#define GSI_ERR_OK                           0               /**< There is no error */
#define GSI_ERR_BUSY                        -1               /**< Busy */
#define GSI_ERR_TIMEOUT                     -3               /**< Timed out */
#define GSI_ERR_FULL                        -4               /**< The resource is full */
#define GSI_ERR_EMPTY                       -5               /**< The resource is empty */
#define GSI_ERR_INVAL                       -6               /**< Invalid argument */
#define GSI_ERR_NULL                        -7
 

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __CLANG_ARM
#endif

/* Compiler Related Definitions */
#if defined(__CC_ARM) || defined(__CLANG_ARM)           /* ARM Compiler */
    #include <stdarg.h>
    #define GSI_SECTION(x)                  __attribute__((section(x)))
    #define GSI_UNUSED                      __attribute__((unused))
    #define GSI_USED                        __attribute__((used))
    #define GSI_ALIGN(n)                    __attribute__((aligned(n)))

    #define GSI_WEAK                        __attribute__((weak))
    #define GSI_INLINE                      static __inline

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define GSI_SECTION(x)                  @ x
    #define GSI_UNUSED
    #define GSI_USED                     	  __root
    #define GSI_PRAGMA(x)                   _Pragma(#x)
    #define GSI_ALIGN(n)                    PRAGMA(data_alignment=n)

    #define GSI_WEAK                       __weak
    #define GSI_INLINE                      static inline

#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #ifdef USING_NEWLIB
        #include <stdarg.h>
    #else
        /* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list       __gnuc_va_list;
        typedef __gnuc_va_list          va_list;
        #define va_start(v,l)           __builtin_va_start(v,l)
        #define va_end(v)               __builtin_va_end(v)
        #define va_arg(v,l)             __builtin_va_arg(v,l)
    #endif

    #define GSI_SECTION(x)               __attribute__((section(x)))
    #define GSI_UNUSED                   __attribute__((unused))
    #define GSI_USED                     __attribute__((used))
    #define GSI_ALIGN(n)                 __attribute__((aligned(n)))
    #define GSI_WEAK                     __attribute__((weak))
    #define GSI_INLINE                   static __inline
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define GSI_SECTION(x)
    #define GSI_UNUSED
    #define GSI_USED
    #define GSI_PRAGMA(x)                   _Pragma(#x)
    #define GSI_ALIGN(n)
    #define GSI_WEAK
    #define GSI_INLINE                      static inline
#else
    #error not supported tool chain
#endif

			  
#define rt_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

			  
/**
 * @ingroup BasicDef
 *
 * @def GSI_SIZE_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. GSI_SIZE_ALIGN(13, 4)
 * would return 16.
 */
#define GSI_SIZE_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def GSI_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. GSI_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define GSI_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))
     
 
#define s_memcpy           memcpy
#define s_memcmp           memcmp
			  
#define s_strcpy           strcpy
#define s_strcmp           strcmp			
			  
	  
extern void hw_interrupt_enter(void);
extern void hw_interrupt_leave(void);
extern void hw_interrupt_enable(void);
extern void hw_interrupt_disable(void);
	  
#define interrupt_enter	     hw_interrupt_enter 
#define interrupt_leave	     hw_interrupt_leave 
#define interrupt_enable     hw_interrupt_enable		  
#define interrupt_disable    hw_interrupt_disable	
#define GSI_ASSERT(...)   

#endif
////////////////////////////////////////////////////////////////////////////
//
//  File        :  arm_arch.lsl
//
//  Version     :  @(#)arm_arch.lsl     1.4  09/04/17
//
//  Description :  Generic LSL file for ARM architectures
//
//  Copyright 2008-2009 Altium BV
//
////////////////////////////////////////////////////////////////////////////

#ifndef __STACK
#  define __STACK                       32k
#endif
#ifndef __HEAP
#  define __HEAP                        32k
#endif
#ifndef __STACK_FIQ
#  define __STACK_FIQ                   8
#endif
#ifndef __STACK_IRQ
#  define __STACK_IRQ                   8
#endif
#ifndef __STACK_SVC
#  define __STACK_SVC                   8
#endif
#ifndef __STACK_ABT
#  define __STACK_ABT                   8
#endif
#ifndef __STACK_UND
#  define __STACK_UND                   8
#endif
#ifndef __PROCESSOR_MODE
#  define __PROCESSOR_MODE              0x1F            /* SYS mode */
#endif
#ifndef __IRQ_BIT
#  define __IRQ_BIT                     0x80            /* IRQ interrupts disabled */
#endif
#ifndef __FIQ_BIT
#  define __FIQ_BIT                     0x40            /* FIQ interrupts disabled */
#endif

#define __APPLICATION_MODE              (__PROCESSOR_MODE | __IRQ_BIT | __FIQ_BIT)

#ifndef __VECTOR_TABLE_ROM_ADDR
#  define __VECTOR_TABLE_ROM_ADDR       0x00000000
#endif

#ifndef __VECTOR_TABLE_RAM_ADDR
#  define __VECTOR_TABLE_RAM_ADDR       0x00000000
#endif

#if defined(__CPU_ARMV7M__) || defined(__CPU_ARMV6M__)
#  ifndef __NR_OF_VECTORS
#    define __NR_OF_VECTORS             16
#  endif
#  define __VECTOR_TABLE_SIZE           (__NR_OF_VECTORS * 4)
#else
#  ifdef __PIC_VECTORS
#    define __VECTOR_TABLE_SIZE         64
#  else
#    ifdef __FIQ_HANDLER_INLINE
#      define __VECTOR_TABLE_SIZE       28
#      define __NR_OF_VECTORS           7
#    else
#      define __VECTOR_TABLE_SIZE       32
#      define __NR_OF_VECTORS           8
#    endif
#  endif
#endif

#ifndef __VECTOR_TABLE_RAM_SPACE
#  undef __VECTOR_TABLE_RAM_COPY
#endif

#ifndef __XVWBUF
#  define __XVWBUF                      0               /* buffer used by CrossView Pro */
#endif

#define BOUNDS_GROUP_NAME       grp_bounds
#define BOUNDS_GROUP_SELECT     "bounds"

architecture ARM
{
        endianness
        {
                little;
                big;
        }

        space linear
        {
                id = 1;
                mau = 8;
                map (size = 4G, dest = bus:local_bus);

                copytable
                (
                        align = 4,
                        copy_unit = 1,
                        dest = linear
                );

                start_address
                (
                        // It is not strictly necessary to define a run_addr for _START
                        // because hardware starts execution at address 0x0 which should
                        // be the vector table with a jump to the relocatable _START, but
                        // an absolute address can prevent the branch to be out-of-range.
                        // Or _START may be the entry point at reset and the reset handler
                        // copies the vector table to address 0x0 after some ROM/RAM memory
                        // re-mapping. In that case _START should be at a fixed address
                        // in ROM, specifically the alias of address 0x0 before memory
                        // re-mapping.
#ifdef __START
                        run_addr = __START,
#endif
                        symbol = "_START"
                );

                stack "stack"
                (
#ifdef __STACK_FIXED
                        fixed,
#endif
                        align = 4,
                        min_size = __STACK,
                        grows = high_to_low
                );

                heap "heap"
                (
#ifdef __HEAP_FIXED
                        fixed,
#endif
                        align = 4,
                        min_size=__HEAP
                );

#if !defined(__CPU_ARMV7M__) && !defined(__CPU_ARMV6M__)
                stack "stack_fiq"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_FIQ,
                        grows = high_to_low
                );
                stack "stack_irq"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_IRQ,
                        grows = high_to_low
                );
                stack "stack_svc"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_SVC,
                        grows = high_to_low
                );
                stack "stack_abt"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_ABT,
                        grows = high_to_low
                );
                stack "stack_und"
                (
                        fixed,
                        align = 4,
                        min_size = __STACK_UND,
                        grows = high_to_low
                );
#endif

#if !defined(__NO_AUTO_VECTORS) && !defined(__NO_DEFAULT_AUTO_VECTORS)
# if defined(__CPU_ARMV7M__) || defined(__CPU_ARMV6M__)
                // vector table with handler addresses
                vector_table "vector_table" ( vector_size = 4, size = __NR_OF_VECTORS, run_addr = __VECTOR_TABLE_ROM_ADDR,
                                              template = ".text.handler_address",
                                              template_symbol = "_lc_vector_handler",
                                              vector_prefix = "_vector_",
                                              fill = loop,
                                              no_inline
                                            )
                {
                        vector ( id = 0, fill = "_START" ); // FIXME: "_lc_ub_stack" does not work
                        vector ( id = 1, fill = "_START" );
                }
#  else
#    ifdef __PIC_VECTORS
                // vector table with ldrpc instructions from handler table
                vector_table "vector_table" ( vector_size = 4, size = 8, run_addr = __VECTOR_TABLE_ROM_ADDR,
                                              template = ".text.vector_ldrpc",
                                              template_symbol = "_lc_vector_ldrpc",
                                              vector_prefix = "_vector_ldrpc_",
                                              fill = loop
                                            )
                {
                }
                // subsequent vector table (data pool) with addresses of handlers
                vector_table "handler_table" ( vector_size = 4, size = 8, run_addr = __VECTOR_TABLE_ROM_ADDR + 32,
                                               template = ".text.handler_address",
                                               template_symbol = "_lc_vector_handler",
                                               vector_prefix = "_vector_",
                                               fill = loop[-32],
                                               no_inline
                                             )
                {
                        vector ( id = 0, fill = "_START" );
                }
#    else
                // vector table with branch instructions to handlers
                vector_table "vector_table" ( vector_size = 4, size = __NR_OF_VECTORS, run_addr = __VECTOR_TABLE_ROM_ADDR,
                                              template = ".text.vector_branch",
                                              template_symbol = "_lc_vector_handler",
                                              vector_prefix = "_vector_",
                                              fill = loop
                                            )
                {
                        vector ( id = 0, fill = "_START" );
                }
#    endif
#  endif
#endif
                section_layout
                {
#if defined(__NO_AUTO_VECTORS)
                        "_lc_ub_vector_table" = __VECTOR_TABLE_ROM_ADDR;
                        "_lc_ue_vector_table" = __VECTOR_TABLE_ROM_ADDR + __VECTOR_TABLE_SIZE;
#endif
#ifdef __VECTOR_TABLE_RAM_SPACE
                        // reserve space to copy vector table from ROM to RAM
                        group ( ordered, run_addr = __VECTOR_TABLE_RAM_ADDR )
                                reserved "vector_table_space" ( size = __VECTOR_TABLE_SIZE, attributes = rwx );
#endif
#ifdef __VECTOR_TABLE_RAM_COPY
                        // provide copy address symbols for copy routine
                        "_lc_ub_vector_table_copy" := "_lc_ub_vector_table_space";
                        "_lc_ue_vector_table_copy" := "_lc_ue_vector_table_space";
#else
                        // prevent copy: copy address equals orig address
                        "_lc_ub_vector_table_copy" := "_lc_ub_vector_table";
                        "_lc_ue_vector_table_copy" := "_lc_ue_vector_table";
#endif
                        // define buffer for string input via Crossview Pro debugger
                        group ( align = 4 ) reserved "xvwbuffer"  (size=__XVWBUF, attributes=rw );

                        // define labels for bounds begin and end as used in C library
#ifndef BOUNDS_GROUP_REDEFINED
                        group BOUNDS_GROUP_NAME (ordered, contiguous)
                        {
                                select BOUNDS_GROUP_SELECT;
                        }
#endif
                        "_lc_ub_bounds" := addressof(group:BOUNDS_GROUP_NAME);
                        "_lc_ue_bounds" := addressof(group:BOUNDS_GROUP_NAME) + sizeof(group:BOUNDS_GROUP_NAME);

#ifdef __HEAPADDR
                        group ( ordered, run_addr=__HEAPADDR )
                        {
                                select "heap";
                        }
#endif
#ifdef __STACKADDR
                        group ( ordered, run_addr=__STACKADDR )
                        {
                                select "stack";
                        }
#endif
#if !defined(__CPU_ARMV7M__) && !defined(__CPU_ARMV6M__)
                        // symbol to set mode bits and interrupt disable bits
                        // in cstart module before calling the application (main)
                        "_APPLICATION_MODE_" = __APPLICATION_MODE;
#endif
                }
        }

        bus local_bus
        {
                mau = 8;
                width = 32;
        }
}

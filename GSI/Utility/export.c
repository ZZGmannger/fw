
#include "export.h"



/*auto init--------------------------------------------------------------------*/
#ifdef USING_INIT_EXPORT
/*
 * Components Initialization will initialize some driver and components as following
 * order:
 * _start         --> 0
 * BOARD_EXPORT      --> 1
 * _board_end     --> 1.end
 *
 * DEVICE_EXPORT     --> 2
 * COMPONENT_EXPORT  --> 3
 * APP_EXPORT        --> 4
 * _end           --> 4.end
 */
static int _start(void)
{
    return 0;
}
INIT_EXPORT(_start, "0");

static int _board_start(void)
{
    return 0;
}
INIT_EXPORT(_board_start, "0.end");

static int _board_end(void)
{
    return 0;
}
INIT_EXPORT(_board_end, "1.end");

static int _end(void)
{
    return 0;
}
INIT_EXPORT(_end, "4.end");



/*for run*/
static void _test_start(void)
{
	
}
RUN_EXPORT(_test_start , "7.end");

static void _test_end(void)
{
	
}
RUN_EXPORT(_test_end , "8.end");


#endif 

void board_auto_init(void)
{
#ifdef USING_INIT_EXPORT
    const init_fn_t *fn_ptr;

    for (fn_ptr = &__init_board_start; fn_ptr < &__init_board_end; fn_ptr++)
    {
        (*fn_ptr)();
    }
#endif
}
 
void components_auto_init(void)
{
#ifdef USING_INIT_EXPORT
    const init_fn_t *fn_ptr;

    for (fn_ptr = &__init_board_end; fn_ptr < &__init_end; fn_ptr ++)
    {
        (*fn_ptr)();
    }
#endif
}

void test_auto_run(void)
{
#ifdef USING_INIT_EXPORT
    const run_fn_t *fn_ptr;

    for (fn_ptr = &__run_test_start; fn_ptr < &__run_test_end; fn_ptr ++)
    {
        (*fn_ptr)();
    }
#endif
}


/*end--------------------------------------------------------------------*/

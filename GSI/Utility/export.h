
#ifndef EXPORT_H
#define EXPORT_H

#include  "gsi_def.h"

/* initialization export */	
#ifdef USING_INIT_EXPORT
	typedef int (*init_fn_t)(void);
	typedef void (*run_fn_t)(void);
	#define INIT_EXPORT(fn, level)						\
            GSI_USED const init_fn_t __init##fn GSI_SECTION(".init_fn." level) = fn
			  
	#define RUN_EXPORT(fn, level)						\
            GSI_USED const run_fn_t  __run##fn GSI_SECTION(".run_fn." level) = fn
#else
	#define INIT_EXPORT(fn, level)
	#define RUN_EXPORT(fn, level)
#endif


#define INIT_BOARD_EXPORT(fn)           INIT_EXPORT(fn, "1")

#define INIT_COMPONENT_EXPORT(fn)       INIT_EXPORT(fn, "2")

#define INIT_APP_EXPORT(fn)             INIT_EXPORT(fn, "3")
			  
#define INIT_TEST_EXPORT(fn)            INIT_EXPORT(fn, "4")
			  
#define RUN_TEST_EXPORT(fn)            	RUN_EXPORT(fn,  "8")

void board_auto_init(void);
void components_auto_init(void);
void test_auto_run(void);

#endif
/*
 * sparse/smatch.h
 *
 * Copyright (C) 2006 Dan Carpenter.
 *
 *  Licensed under the Open Software License version 1.1
 *
 */

#ifndef   	SMATCH_H_
# define   	SMATCH_H_

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "lib.h"
#include "allocate.h"
#include "parse.h"
#include "expression.h"

#define KERNEL

struct state_history {
	unsigned int loc;
};
DECLARE_PTR_LIST(history_list, struct state_history);

struct smatch_state {
        char *name;
	unsigned int owner;
	struct symbol *sym;
  	int state;
	struct history_list *line_history;
	struct history_list *path_history;
};

enum hook_type {
	EXPR_HOOK,
	STMT_HOOK,
	SYM_HOOK,
	DECLARATION_HOOK,
	ASSIGNMENT_HOOK,
	ASSIGNMENT_AFTER_HOOK,
	CONDITION_HOOK,
	WHOLE_CONDITION_HOOK,
	FUNCTION_CALL_HOOK,
	FUNCTION_CALL_AFTER_HOOK,
	DEREF_HOOK,
	BASE_HOOK,
	FUNC_DEF_HOOK,
	END_FUNC_HOOK,
	RETURN_HOOK,
};
void add_hook(void *func, enum hook_type type);
typedef int (merge_func_t)(const char *name, struct symbol *sym, int s1, 
			   int s2);
void add_merge_hook(int client_id, merge_func_t *func);

#define smatch_msg(msg...) \
do {                                                          \
	printf("%s %d %s(%d) ", get_filename(), get_lineno(), \
	       get_function(), get_func_pos());               \
        printf(msg);                                          \
        printf("\n");                                         \
} while (0)

#define SM_DEBUG(msg...) do { if (debug_states) printf(msg); } while (0)

#define NOTFOUND -2
#define UNDEFINED -1

int get_state(const char *name, int owner, struct symbol *sym);
void add_state(const char *name, int owner, struct symbol *sym, int state);
void set_state(const char *name, int owner, struct symbol *sym, int state);
void delete_state(const char *name, int owner, struct symbol *sym);
void set_true_false_states(const char *name, int owner, struct symbol *sym, 
			   int true_state, int false_state);
int state_defined(const char *name, int owner, struct symbol *sym);

struct state_list *get_all_states();
void nullify_path();	   
void clear_all_states();

char *get_filename();
char *get_function();
int get_lineno();
int get_func_pos();

/* smatch_helper.c */
char *alloc_string(char *str);
void free_string(char *str);
struct expression *get_argument_from_call_expr(struct expression_list *args,
					       int num);
char * get_variable_from_expr(struct expression * expr,
			      struct symbol **sym_ptr);
char * get_variable_from_expr_simple(struct expression * expr,
			      struct symbol **sym_ptr);
int sym_name_is(const char *name, struct expression *expr);
int get_value(struct expression *expr, int *discard);
int is_zero(struct expression *expr);

/* ----------------------------------------------------------------
   The stuff below is all used internally and shouldn't 
   be called from other programs 
 -----------------------------------------------------------------*/

/* smatch_flow.c */

void smatch (int argc, char **argv);
void __split_expr(struct expression *expr);

/* smatch_conditions */
void __split_whole_condition(struct expression *expr);

/* smatch_extras.c */
int known_condition_true(struct expression *expr);

/* smatch_states.c */

extern int debug_states;

void __prep_false_only_stack();
void __use_false_only_stack();
void __use_true_states();
void __use_false_states();
void __pop_false_states();
void __merge_false_states();
void __merge_true_states();
void __pop_true_states();

void __negate_cond_stacks();
void __use_cond_true_states();
void __use_cond_false_states();
void __push_cond_stacks();
void __and_cond_states();
void __or_cond_states();
void __save_pre_cond_states();
void __pop_pre_cond_states();
void __use_cond_states();

void __push_continues();
void __pop_continues();
void __process_continues();
void __merge_continues();

void __push_breaks();
void __pop_breaks();
void __process_breaks();
void __merge_breaks();

void __save_switch_states();
void __pop_switches();
void __merge_switches();
void __push_default();
void __set_default();
int __pop_default();

void __push_conditions();
void __pop_conditions();
void __overwrite_true_states();

void __save_gotos(const char *name);
void __merge_gotos(const char *name);

void __print_cur_slist();

/* smatch_hooks.c */
void __pass_to_client(void *data, enum hook_type type);
void __pass_declarations_to_client(struct symbol_list *sym_list);
int __has_merge_function(int client_id);
int __client_merge_function(int owner, const char *name, struct symbol *sym, 
			    int s1, int s2);

#endif 	    /* !SMATCH_H_ */
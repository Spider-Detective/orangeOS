/*
 * Make variables in global.c does not have extern
 */

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"

PUBLIC PROCESS           proc_table[NR_TASKS];
PUBLIC char              task_stack[STACK_SIZE_TOTAL];
PUBLIC TASK              task_table[NR_TASKS] = {{TestA, STACK_SIZE_TESTA, "TestA"},
                                                 {TestB, STACK_SIZE_TESTB, "TestB"},
                                                 {TestC, STACK_SIZE_TESTC, "TestC"}};
PUBLIC irq_handler       irq_table[NR_IRQ];
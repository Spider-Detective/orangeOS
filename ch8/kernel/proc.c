/*
 * Over all call flow for system call:
 * User process call get_ticks() (main.c)
 * INT_VECTOR_SYS_CALL in get_ticks() will trigger sys_call() in idt table (kernel.asm)
 * sys_call() calls sys_get_ticks() in sys_call_table (proc.c)
 */
#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "console.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "proto.h"

PRIVATE void block(struct proc* p);
PRIVATE void unblock(struct proc* p);
PRIVATE int msg_send(struct proc* current, int dest, MESSAGE* m);
PRIVATE int msg_receive(struct proc* current, int src, MESSAGE* m);
PRIVATE int deadlock(int src, int dest);

/*
 * Function for scheduling the processes
 */
PUBLIC void schedule() {
    struct proc* p;
    int          greatest_ticks = 0;

    while (!greatest_ticks) {
        // switch to the process with largest ticks
        for (p = &LAST_PROC; p <= &LAST_PROC; p++) {
            if (p->p_flags == 0) {
                if (greatest_ticks < p->ticks) {
                    greatest_ticks = p->ticks;
                    p_proc_ready = p;
                }
            }
        }
        // if all processes have 0 ticks, 
        // assign the initial ticks to all processes and switch to the largest one
        if (!greatest_ticks) {
            for (p = &LAST_PROC; p <= &LAST_PROC; p++) {
                if (p->p_flags == 0) {
                    p->ticks = p->priority;
                }
            }
        }
    }
}

/*
 * send/receive msg in kernel <ring 0>
 * function: send or receive
 * src_dest: To/From whom the msg is transferred
 * m: pointer to the msg
 * p: the caller proc
 * return 0 if success
 */
PUBLIC int sys_sendrec(int function, int src_dest, MESSAGE* m, struct proc* p) {
    assert(k_reenter == 0);
    assert((src_dest >= 0 && src_dest < NR_TASKS + NR_PROCS) ||
            src_dest == ANY ||
            src_dest == INTERRUPT);

    int ret = 0;
    int caller = proc2pid(p);
    MESSAGE* mla = (MESSAGE*)va2la(caller, m);
    mla->source = caller;

    assert(mla->source != src_dest);

    if (function == SEND) {
        ret = msg_send(p, src_dest, m);
        if (ret != 0) {
            return ret;
        }
    } else if (function == RECEIVE) {
        ret = msg_receive(p, src_dest, m);
        if (ret != 0) {
            return ret;
        }
    } else {
        panic("{sys_sendrec} invalid function: "
              "%d (SEND:%d, RECEIVE:%d).", function, SEND, RECEIVE);
    }

    return 0;
}

PUBLIC int send_recv(int function, int src_dest, MESSAGE* msg) {
    return 0;
}

// Ring 0: calculate the linear address of a certain seg of a given process
//         by using the descriptor of process
PUBLIC int ldt_seg_linear(struct proc* p, int idx) {
    struct descriptor* d = &p->ldts[idx];
    return d->base_high << 24 | d->base_mid << 16 | d->base_low;
}

// Ring 0: convert virtual/linear address to physical address
PUBLIC void* va2la(int pid, void* va) {
    struct proc* p = &proc_table[pid];

    u32 seg_base = ldt_seg_linear(p, INDEX_LDT_RW);
    u32 la = seg_base + (u32)va;

    if (pid < NR_TASKS + NR_PROCS) {
        assert(la == (u32)va);
    }

    return (void*)la;
}

// clear the msg by filling all 0
PUBLIC void reset_msg(MESSAGE* p) {
    memset(p, 0, sizeof(MESSAGE));
}

PRIVATE void block(struct proc* p) {

}

PRIVATE void unblock(struct proc* p) {

}

PRIVATE int msg_send(struct proc* current, int dest, MESSAGE* m) {
    return 0;
}

PRIVATE int msg_receive(struct proc* current, int src, MESSAGE* m) {
    return 0;
}

PRIVATE int deadlock(int src, int dest) {
    return 0;
}

PUBLIC void dump_proc(struct proc* p) {

}

PUBLIC void dump_msg(const char* title, MESSAGE* m) {

}

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "concurrency.h"

/*
We use a similar implementation of processes as in part 1, with some modification to process_select and process_create.  

To implement wait-free locks, we store a waiting queue within a lock data structure. When a process acquires the lock, it marks the lock as acquired and continues to the critical section. If another process tries to acquire that lock, it will instead mark itself as waiting and then yield. In process_select, we check if the "current" process (the process that just yielded) is marked as waiting. If it is marked as waiting then we will append it to the waiting queue of the lock that the process was waiting for, instead of the ready queue. 

When the process with the lock releases it, it will pop the first process in the waiting queue onto the ready queue, and then mark the lock as free. The released process will then eventually be able to acquire the lock. 
*/

struct process_state
{
	unsigned int sp;						/* stack pointer */
	struct process_state *next; /* link to next process */
	lock_t *waiting;						/* null, or reference to the lock that a process is waiting for */
};

process_t *queue_head = NULL;
process_t *queue_tail = NULL;
process_t *current_process = NULL;
__attribute__((used)) unsigned char _orig_sp_hi, _orig_sp_lo;

__attribute__((used)) void process_begin()
{
	asm volatile(
			"cli \n\t"
			"in r24,__SP_L__ \n\t"
			"sts _orig_sp_lo, r24\n\t"
			"in r25,__SP_H__ \n\t"
			"sts _orig_sp_hi, r25\n\t"
			"ldi r24, 0\n\t"
			"ldi r25, 0\n\t"
			"rjmp .dead_proc_entry\n\t");
}

__attribute__((used)) void process_terminated()
{
	asm volatile(
			"cli\n\t"
			"lds r25, _orig_sp_hi\n\t"
			"out __SP_H__, r25\n\t"
			"lds r24, _orig_sp_lo\n\t"
			"out __SP_L__, r24\n\t"
			"ldi r24, lo8(0)\n\t"
			"ldi r25, hi8(0)\n\t"
			"rjmp .dead_proc_entry");
}

void process_timer_interrupt();

__attribute__((used)) void yield()
{
	if (!current_process)
		return;
	asm volatile("cli\n\t");
	asm volatile("rjmp process_timer_interrupt\n\t");
}

__attribute__((used)) void process_timer_interrupt()
{
	asm volatile(
			"push r31\n\t"
			"push r30\n\t"
			"push r29\n\t"
			"push r28\n\t"
			"push r27\n\t"
			"push r26\n\t"
			"push r25\n\t"
			"push r24\n\t"
			"push r23\n\t"
			"push r22\n\t"
			"push r21\n\t"
			"push r20\n\t"
			"push r19\n\t"
			"push r18\n\t"
			"push r17\n\t"
			"push r16\n\t"
			"push r15\n\t"
			"push r14\n\t"
			"push r13\n\t"
			"push r12\n\t"
			"push r11\n\t"
			"push r10\n\t"
			"push r9\n\t"
			"push r8\n\t"
			"push r7\n\t"
			"push r6\n\t"
			"push r5\n\t"
			"push r4\n\t"
			"push r3\n\t"
			"push r2\n\t"
			"push r1\n\t"
			"push r0\n\t"
			"in r24, __SREG__\n\t"
			"push r24\n\t"
			"in r24, __SP_L__\n\t"
			"in r25, __SP_H__\n\t"
			".dead_proc_entry:\n\t"
			"rcall process_select\n\t"
			"eor r18,r18\n\t"
			"or r18,r24\n\t"
			"or r18,r25\n\t"
			"brne .label_resume\n\t"
			"lds r25, _orig_sp_hi\n\t"
			"out __SP_H__, r25\n\t"
			"lds r24, _orig_sp_lo\n\t"
			"out __SP_L__, r24\n\t"
			"ret\n\t"
			".label_resume:\n\t"
			"out __SP_L__, r24\n\t"
			"out __SP_H__, r25\n\t"
			"pop r0\n\t"
			"out  __SREG__, r0\n\t"
			"pop r0\n\t"
			"pop r1\n\t"
			"pop r2\n\t"
			"pop r3\n\t"
			"pop r4\n\t"
			"pop r5\n\t"
			"pop r6\n\t"
			"pop r7\n\t"
			"pop r8\n\t"
			"pop r9\n\t"
			"pop r10\n\t"
			"pop r11\n\t"
			"pop r12\n\t"
			"pop r13\n\t"
			"pop r14\n\t"
			"pop r15\n\t"
			"pop r16\n\t"
			"pop r17\n\t"
			"pop r18\n\t"
			"pop r19\n\t"
			"pop r20\n\t"
			"pop r21\n\t"
			"pop r22\n\t"
			"pop r23\n\t"
			"pop r24\n\t"
			"pop r25\n\t"
			"pop r26\n\t"
			"pop r27\n\t"
			"pop r28\n\t"
			"pop r29\n\t"
			"pop r30\n\t"
			"pop r31\n\t"
			"reti\n\t");
}

/*
 * Stack: save 32 regs, +2 for entry point +2 for ret address
 */
#define EXTRA_SPACE 37
#define EXTRA_PAD 4

unsigned int process_init(void (*f)(void), int n)
{
	unsigned long stk;
	int i;
	unsigned char *stkspace;

	/* Create a new process */
	n += EXTRA_SPACE + EXTRA_PAD;
	stkspace = (unsigned char *)malloc(n);

	if (stkspace == NULL)
	{
		/* failed! */
		return 0;
	}

	/* Create the "standard" stack, including entry point */
	for (i = 0; i < n; i++)
	{
		stkspace[i] = 0;
	}

	n -= EXTRA_PAD;

	stkspace[n - 1] = ((unsigned int)process_terminated) & 0xff;
	stkspace[n - 2] = ((unsigned int)process_terminated) >> 8;
	stkspace[n - 3] = ((unsigned int)f) & 0xff;
	stkspace[n - 4] = ((unsigned int)f) >> 8;

	/* SREG */
	stkspace[n - EXTRA_SPACE] = SREG;

	stk = (unsigned int)stkspace + n - EXTRA_SPACE - 1;

	return stk;
}

int process_create(void (*f)(void), int n)
{
	// Allocate stack space for new process
	int sp = process_init(f, n);

	// Return error if failed to allocate stack space for new process
	if (sp == 0)
	{
		return -1;
	}

	// Create linked list node for new process
	process_t *new_process = (process_t *)malloc(sizeof(process_t));
	if (new_process == NULL)
	{
		return -1;
	}
	new_process->sp = sp;
	new_process->next = NULL;
	new_process->waiting = NULL;

	// Check if queue is empty and adding first node
	if (queue_head == NULL)
	{
		queue_head = new_process;
	}
	// Otherwise append to end of queue
	else
	{
		queue_tail->next = new_process;
	}
	// Set end of queue to the new node
	queue_tail = new_process;

	return 0;
}

void process_start(void)
{
	// Indirectly call process select
	process_begin();
}

__attribute__((used)) unsigned int process_select(unsigned int cursp)
{
	// Nothing in the queue
	if (queue_head == NULL)
	{
		// No running process
		if (cursp == 0)
		{
			return 0;
		}
		// Some running process
		else
		{
			// Continue running the same process with the current stack pointer. Technically we shouldn't need to have to update the process' sp since we're not appending it to anything right now, but we do it for consistency's sake.
			current_process->sp = cursp;
			return cursp;
		}
	} // Else something in the queue

	// Some running process, so check which queue to append to based on if its waiting or not
	if (cursp != 0)
	{
		// Save current stack pointer as starting point when next run
		current_process->sp = cursp;
		// Current process is not waiting on any lock, so append to ready queue
		if (current_process->waiting == NULL)
		{
			queue_tail->next = current_process;
			queue_tail = current_process;
		}
		else
		{ // Current process is waiting so add to lock's waiting queue
			lock_t *l = current_process->waiting;
			if (l->q_head == NULL)
			{
				l->q_head = current_process;
				l->q_tail = current_process;
			}
			else
			{
				l->q_tail->next = current_process;
				l->q_tail = current_process;
			}
		}
	}

	// Pop next process to run
	current_process = queue_head;
	queue_head = queue_head->next;
	current_process->next = NULL;

	// Check if we emptied the queue, update tail as necessary.
	if (queue_head == NULL)
	{
		queue_tail = NULL;
	}

	return current_process->sp;
}

void lock_init(lock_t *l)
{
	l->acquired = 0;
	l->q_head = NULL;
	l->q_tail = NULL;
}

void lock_acquire(lock_t *l)
{
	// We need to mutex this section to ensure that we don't get two or more processes thinking they have acquired the lock. So we disable interrupts until we are about to yield, or after we have marked the lock as acquired.
	cli();
	while (l->acquired == 1) // We use while instead of if, in case this process p2 goes to wait and then tries again when the lock is released but some other process p3 has obtained it first (for example if p3 was initialized and added to the queue while p2 was waiting). If we want to ensure p2 goes next we could try to append to the head instead.
	{
		// Since we need to update the current stack pointer of this process before we add it to the waiting queue, we append it only in process_select, where we have access to cursp. But we need process_select to have access to this lock's waiting queue, so we save a reference to this lock in this process.
		current_process->waiting = l;
		sei();
		yield();
	}
	// Mark the lock as acquired.
	l->acquired = 1;
	sei();
	return;
}

void lock_release(lock_t *l)
{
	// If there is anybody waiting, pop the head of this lock's waiting queue onto the ready queue
	if (l->q_head != NULL)
	{
		// We mutually exclude this section since things may get messy if interrupted.
		// For instance, if we append a waiting process onto the tail of the ready queue but haven't set the new tail reference, and this current process gets interrupted, it will make itself the new tail, thereby overwriting the pointer to the released process.
		cli();
		process_t *next = l->q_head->next;

		// Mark the lock as free
		l->acquired = 0;

		// Mark the head as non-waiting
		l->q_head->waiting = NULL;

		// Empty queue
		if (queue_tail == NULL)
		{
			// Add as head of ready queue
			queue_head = l->q_head;
		}
		else
		{
			// Append to tail of ready queue
			queue_tail->next = l->q_head;
		}
		// Set new tail of ready queue
		queue_tail = l->q_head;
		// Set new head of waiting queue
		l->q_head = next;
		sei();
	}
	else
	{
		// Mark the lock as free
		l->acquired = 0;
	}
	// Yield so that another process (for example the just-released process) could try to acquire the lock. This helps improve fairness, particularly in the case (as it is with our current test) when the current running process might immediately try to acquire the lock again, and thus starve other waiting processes.
	yield();
}

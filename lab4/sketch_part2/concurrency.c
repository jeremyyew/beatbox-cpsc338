#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "concurrency.h"

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
			current_process->sp = cursp;
			return cursp;
		}
	}
	// Else something in the queue

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
	cli();
	while (l->acquired == 1)
	{
		current_process->waiting = l;
		sei();
		yield();
	}

	l->acquired = 1;
	sei();
	return;
}

void lock_release(lock_t *l)
{
	// If there is anybody waiting, pop the head of this lock's waiting queue onto the ready queue.
	if (l->q_head != NULL)
	{
		cli();
		process_t *next = l->q_head->next;

		l->acquired = 0;
		l->q_head->waiting = NULL;

		// Empty queue
		if (queue_tail == NULL)
		{
			queue_head = l->q_head;
			queue_tail = l->q_head;
		}
		else
		{
			queue_tail->next = l->q_head;
			queue_tail = queue_tail->next;
		}
		l->q_head = next;
		sei();
	}
	else
	{
		l->acquired = 0;
	}
	// yield so the just queued process can try to acquire the lock to be fair
	yield();
}

#ifndef PARLIB_ARCH_H
#define PARLIB_ARCH_H

#include <ros/common.h>

static __inline void
breakpoint(void)
{
	asm volatile ("ta 0x7f");
}

static __inline uint64_t
read_perfctr(uint32_t cpu, uint32_t which)
{
	register uint32_t hi asm("o0"), lo asm("o1");
	uintptr_t addr = cpu<<10 | which<<3;

	asm volatile("mov %2,%%o0; ta 9"
	             : "=r"(hi),"=r"(lo) : "r"(addr));
	return (((uint64_t)hi) << 32) | lo;
}

static __inline uint64_t
read_tsc(void)
{
	return read_perfctr(0,0);
}

static __inline void
cpu_relax(void)
{
	int ctr = 8;
	asm volatile("1: deccc %0; bne 1b; nop" :
	             "=r"(ctr) : "0"(ctr) : "cc","memory");
}
#endif /* PARLIB_ARCH_H */
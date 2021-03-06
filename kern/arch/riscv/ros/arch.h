#ifndef _ROS_ARCH_ARCH_H
#define _ROS_ARCH_ARCH_H

#define MAX_NUM_CPUS				16 // it's safe to change this as needed

#ifndef __ASSEMBLER__

static __inline unsigned int read_fsr(void)
{
	unsigned int fsr;
	asm volatile ("mffsr %0" : "=r"(fsr));
	return fsr;
}

static __inline void write_fsr(unsigned int fsr)
{
	asm volatile ("mtfsr %0" :: "r"(fsr));
}

#endif /* !__ASSEMBLER__ */

#endif

#ifndef UTIL_H
#define UTIL_H

#ifndef __ASSEMBLER__

#define BIT_MASK(OFF, LEN) (((1ULL << (LEN)) - 1) << (OFF))

#endif /* __ASSEMBLER__ */

#endif /* UTIL_H */
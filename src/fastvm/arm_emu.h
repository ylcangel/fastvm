﻿
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef __arm_emu_h__
#define __arm_emu_h__


#define ARM_REG_R0      0
#define ARM_REG_R1      1
#define ARM_REG_R2      2
#define ARM_REG_R3      3
#define ARM_REG_R4      4
#define ARM_REG_R5      5
#define ARM_REG_R6      6
#define ARM_REG_R7      7
#define ARM_REG_R8      8
#define ARM_REG_R9      9
#define ARM_REG_R10     10
#define ARM_REG_R11     11
#define ARM_REG_R12     12
#define ARM_REG_R13     13
#define ARM_REG_R14     14
#define ARM_REG_R15     15
#define ARM_REG_R16     16

#define SYS_REG_NUM     32


#define ARM_REG_PC      ARM_REG_R15
#define ARM_REG_LR      ARM_REG_R14
#define ARM_REG_SP      ARM_REG_R13
#define ARM_REG_APSR    ARM_REG_R16

#define ARM_COND_EQ     0
#define ARM_COND_NE     1
#define ARM_COND_CS     2
#define ARM_COND_CC     3
#define ARM_COND_MI     4
#define ARM_COND_PL     5
#define ARM_COND_VS     6
#define ARM_COND_VC     7
#define ARM_COND_HI     8
#define ARM_COND_LS     9
#define ARM_COND_GE     10
#define ARM_COND_LT     11
#define ARM_COND_GT     12
#define ARM_COND_LE     13
#define ARM_COND_AL     14
#define ARM_COND_AL2    15

#define KB          (1024)
#define MB          (1024 * 1024)

typedef int         reg_t;

struct arm_cpsr {
    unsigned m:  5;
    unsigned t : 1;
    unsigned f : 1;
    unsigned i : 1;
    unsigned a : 1;
    unsigned e : 1;
    unsigned it1 : 6;
    unsigned ge : 4;
    unsigned reserved : 4;
    unsigned j : 1;
    unsigned it2 : 2;
    unsigned q : 1;
    unsigned v : 1;
    unsigned c : 1;
    unsigned z : 1;
    unsigned n : 1;
};

struct arm_inst_ctx {
    reg_t   ld;     // 目的寄存器
    reg_t   ld2;    // 目的寄存器2
    reg_t   lm;     // 参数寄存器1
    reg_t   ln;     // 参数寄存器2
    reg_t   lp;     // 参数寄存器3
    int     register_list;  // 寄存器列表，一般给push和pop用
    int     imm;    // 立即数
    int     m; 
    int     setflags;   // 是否需要跟新flag
    int     cond;       // 条件
    int     vd;         // simd中需要

    struct {
        unsigned p : 1;
        unsigned u : 1;     // 在一些立即数中，指出是加还是减
        unsigned w : 1;
        unsigned t : 2;
    };
};

    /* 在机器上运行模拟器时，自己的机器叫host，模拟器里的环境叫target*/
struct arm_emu_create_param
{
    char *filename;
    unsigned char*  elf;
    int             elf_len;

    unsigned char*  code;
    int             code_len;
    void            *user_ctx;
    int(*inst_func)(unsigned char *inst, int len,  char *inst_str, void *user_ctx);

    /* is thumb instruction */
    int             thumb;
};

struct arm_emu*         arm_emu_create(struct arm_emu_create_param *param);
void                    arm_emu_destroy(struct arm_emu *);

/*

 @return    0        success
            1        finish
            <0        error
*/
int arm_emu_run(struct arm_emu *vm);

/* 从指令中获取关键性上下文 */
int arm_inst_extract_ctx(struct arm_inst_ctx *ctx, const char *exp, uint8_t *code, int code_len);

/*
arm assembly to binary code
@olen       bincode len
@return     != NULL     success
            
*/
char* arm_asm2bin(char *bin, int *olen, const char *asm, ...);
const char* arm_reg2str(int reg);

#endif /* __arm_emu_h__ */

#if defined(__cplusplus)
}
#endif
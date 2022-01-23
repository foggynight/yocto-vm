// stack.c - yocto-vm stack machine.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE (2 << 16)

typedef enum inst {
    INST_NOP,
    INST_PUSH,
    INST_POP,
    INST_TOP,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV
} inst_t;
typedef int data_t;

data_t stack[STACK_SIZE];
size_t stack_ptr;

void error(const char *msg) {
    fprintf(stderr, "stack: %s\n", msg);
    exit(1);
}

void stack_push(data_t d) {
    if (stack_ptr >= STACK_SIZE) error("stack overflow");
    stack[stack_ptr++] = d;
}

data_t stack_pop(void) {
    if (stack_ptr <= 0) error("stack underflow");
    return stack[--stack_ptr];
}

data_t stack_top(void) {
    if (stack_ptr <= 0) error("stack underflow");
    return stack[stack_ptr - 1];
}

void print_stack(void) {
    for (size_t i = 0; i < stack_ptr; ++i)
        printf("%d ", stack[i]);
}

inst_t str_to_inst(const char *str) {
    if      (!strcmp(str, "nop"))  return INST_NOP;
    else if (!strcmp(str, "push")) return INST_PUSH;
    else if (!strcmp(str, "pop"))  return INST_POP;
    else if (!strcmp(str, "top"))  return INST_TOP;
    else if (!strcmp(str, "add"))  return INST_ADD;
    else if (!strcmp(str, "sub"))  return INST_SUB;
    else if (!strcmp(str, "mul"))  return INST_MUL;
    else if (!strcmp(str, "div"))  return INST_DIV;
    else puts("invalid instruction");
    return INST_NOP;
}

char arg_count(inst_t inst) {
    switch (inst) {
    case INST_NOP:  return 0;
    case INST_PUSH: return 1;
    case INST_POP:  return 0;
    case INST_TOP:  return 0;
    case INST_ADD:  return 0;
    case INST_SUB:  return 0;
    case INST_MUL:  return 0;
    case INST_DIV:  return 0;
    }
    return 0;
}

long str_to_long(const char *str) {
    long acc = 0;
    long walk = 1;
    for (size_t i = 0; i < strlen(str); ++i) {
        acc += (str[strlen(str)-i-1] - '0') * walk;
        walk *= 10;
    }
    return acc;
}

void inst_add(void) {
    if (stack_ptr < 2) error("stack underflow");
    data_t pop = stack_pop();
    stack[stack_ptr-1] += pop;
}

void inst_sub(void) {
    if (stack_ptr < 2) error("stack underflow");
    data_t pop = stack_pop();
    stack[stack_ptr-1] -= pop;
}

void inst_mul(void) {
    if (stack_ptr < 2) error("stack underflow");
    data_t pop = stack_pop();
    stack[stack_ptr-1] *= pop;
}

void inst_div(void) {
    if (stack_ptr < 2) error("stack underflow");
    data_t pop = stack_pop();
    stack[stack_ptr-1] /= pop;
}

void exec_inst(inst_t inst, data_t arg) {
    switch (inst) {
    case INST_NOP:                   break;
    case INST_PUSH: stack_push(arg); break;
    case INST_POP:  stack_pop();     break;
    case INST_TOP:  stack_top();     break;
    case INST_ADD:  inst_add();      break;
    case INST_SUB:  inst_sub();      break;
    case INST_MUL:  inst_mul();      break;
    case INST_DIV:  inst_div();      break;
    }
}

int main(void) {
    char inst_str[1024];
    char arg_str[1024];
    printf("> ");
    while (scanf("%1023s", inst_str) == 1) {
        inst_t inst = str_to_inst(inst_str);
        data_t arg = 0;
        if (arg_count(inst)) {
            if (scanf("%1023s", arg_str) != 1)
                error("unable to fetch argument");
            arg = str_to_long(arg_str);
        }
        print_stack();
        exec_inst(inst, arg);
        printf("-> ");
        print_stack();
        printf("\n> ");
    }
}

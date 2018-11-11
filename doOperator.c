#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "doOperator.h"
#include "tokenStack.h"
#include "lexical.h"

static int op_quit(struct tokenStack *stack);
static int op_print(struct tokenStack *stack);
static int op_dump(struct tokenStack *stack);
static int op_add(struct tokenStack *stack);

static int op_sub(struct tokenStack *stack);
static int op_prod(struct tokenStack *stack);
static int op_drop(struct tokenStack *stack);
static int op_mod(struct tokenStack *stack);
static int op_div(struct tokenStack *stack);
static int op_if(struct tokenStack *stack);
static int op_s(struct tokenStack *stack);
static int op_eq(struct tokenStack *stack);
static int op_swap(struct tokenStack *stack);
static int op_modquot(struct tokenStack *stack);
static int op_gt(struct tokenStack *stack);
static int op_lt(struct tokenStack *stack);

static struct operator_struct
{
  char *name;
  int (*fn_ptr)(struct tokenStack *);
} ops[] = {
    {"quit", op_quit},
    {"print", op_print},
    {"dump", op_dump},
    {"+", op_add},
    {"-", op_sub},
    {"*", op_prod},
    {"/", op_div},
    {"DROP", op_drop},
    {"MOD", op_mod},
    {"IF", op_if},
    {"S", op_s},
    {"EQ", op_eq},
    {"SWAP", op_swap},
    {"MODQUOT", op_modquot},
    {"GT", op_gt},
    {"LT", op_lt},
    {(char *)NULL, (int (*)(struct tokenStack *))NULL}};

/* YOU WRITE THIS */
static int popInt(struct tokenStack *s)
{
  int pop_val = 0;
  if (s->top == 0)
  {
    fprintf(stderr, "Cannot pop empty stack!\n");
    exit(1);
  }
  else
  {
    pop_val = atoi (popTokenStack(s)->symbol);
  }
  return pop_val;
}

/* YOU WRITE THIS */
static void pushInt(struct tokenStack *s, int v)
{
  struct lexToken *token;
  token = allocToken();

  token->kind = LEX_TOKEN_NUMBER;

  sprintf(token->symbol, "%d", v);
  pushTokenStack(s, token);
  op_print(s);
}

int doOperator(struct tokenStack *stack, char *o)
{
  struct operator_struct *op = ops;
  for (op = ops; op->name != (char *)NULL; op++)
  {
    if (!strcmp(op->name, o))
      return op->fn_ptr(stack);
  }
  return (-1);
}

/*ARGSUSED*/
static int op_quit(struct tokenStack *stack)
{
  printf("[quit]\n");
  exit(0);
  /*NOTREACHED*/
}

static int op_print(struct tokenStack *stack)
{
  struct lexToken *t = popTokenStack(stack);
  printToken(stdout, t);
  freeToken(t);
  return (0);
}

static int op_dump(struct tokenStack *stack)
{
  struct lexToken *t = popTokenStack(stack);
  dumpToken(stdout, t);
  freeToken(t);
  return (0);
}

static int op_add(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v1 + v2);
  return (0);
}

static int op_sub(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v2 - v1);
  return (0);
}

static int op_prod(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v1 * v2);
  return (0);
}

static int op_div(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v1 / v2);
  return (0);
}

static int op_mod(struct tokenStack *stack)
{
  int v1;
  v1 = popInt(stack);
  pushInt(stack, v1);
  pushInt(stack, v1);
  return (0);
}

static int op_drop(struct tokenStack *stack)
{
  int v1;
  v1 = popInt(stack);
  pushInt(stack, v1);
  return (0);
}

static int op_if(struct tokenStack *stack)
{
  int v, v1, v2;
  v = popInt(stack);
  v1 = popInt(stack);
  v2 = popInt(stack);
  
  if (v != 0)
  {
    pushInt(stack, v1);
  }
  else
  {
    pushInt(stack, v2);
  }
  return (0);
}

static int op_s(struct tokenStack *stack)
{
  int v1;
  while (!emptyTokenStack(stack))
  {
    v1 = popInt(stack);
    pushInt(stack, v1);
  }
  return (0);
}

static int op_swap(struct tokenStack *stack)
{
  int v1, v2;
  /*
  v1 = popInt(stack);
  v2 = popInt(stack);
  pushInt(stack, v2);
  pushInt(stack, v1);
  */
  while (!emptyTokenStack(stack))
  {
    v1 = popInt(stack);
    v2 = popInt(stack);
    pushInt(stack, v2);
    pushInt(stack, v1);
  }
  return (0);
}

static int op_eq(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  if (v1 == v2)
  {
    pushInt(stack, 1);
  }
  else
  {
    pushInt(stack, 0);
  }
  return (0);
}

static int op_modquot(struct tokenStack *stack)
{
  int v1, v2;
  int remainder;
  v1 = popInt(stack);
  v2 = popInt(stack);
  remainder = v1 % v2;
  pushInt(stack, remainder);
  pushInt(stack, v1 / v2);
  return (0);
}

static int op_gt(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  if (v1 > v2)
  {
    pushInt(stack, 1);
  }
  else
  {
    pushInt(stack, 0);
  }
  return (0);
}

static int op_lt(struct tokenStack *stack)
{
  int v1, v2;
  v1 = popInt(stack);
  v2 = popInt(stack);
  if (v1 < v2)
  {
    pushInt(stack, 1);
  }
  else
  {
    pushInt(stack, 0);
  }
  return (0);
}
 
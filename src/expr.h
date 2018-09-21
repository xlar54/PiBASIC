#ifndef _EXPR_H_
#define _EXPR_H

extern "C"
{
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define EXPR_ERR_NONE			0
#define EXPR_ERR_SYNTAX			1
#define EXPR_ERR_DIV_BY_ZERO	2

#define EXPR_TOKEN_AND			1		
#define EXPR_TOKEN_OR			2
#define EXPR_TOKEN_ABS			10

	static unsigned char expr_stack[200][10];
	static int expr_top = -1;

	int expr_eval(unsigned char* expr, double *result);
	int expr_infix_to_postfix(unsigned char *exp, unsigned char*postfix);
	int expr_eval_postfix(unsigned char* postfix, double* result);
	int expr_isalnum(int c);
	void expr_spush(unsigned char *x);
	unsigned char *expr_spop();
	int expr_priority(unsigned char *x);


}



#endif
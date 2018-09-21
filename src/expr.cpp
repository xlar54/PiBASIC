#include "expr.h"
#include "terminal.h"

int expr_eval(unsigned char* expr, double *result)
{
	int error = EXPR_ERR_NONE;
	unsigned char *postfix = (unsigned char *)malloc(sizeof(unsigned char *) * 255);

	error = expr_infix_to_postfix(expr, postfix);

	if (error != EXPR_ERR_NONE)
	{
		free(postfix);
		return error;
	}

	error = expr_eval_postfix(postfix, result);
	free(postfix);
	return error;
}

int expr_isalnum(int c)
{
	unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.~abcdefghijklmnopqrstuvwxyz";
	unsigned char *letter = alphabet;

	while (*letter != '\0' && *letter != c)
		++letter;

	if (*letter)
		return 1;

	return 0;
}

void expr_spush(unsigned char *x)
{
	int ptr = 0;
	expr_top++;
	while (*x != 0)
	{
		expr_stack[expr_top][ptr] = *x;
		x++;
		ptr++;
	}
	expr_stack[expr_top][ptr] = 0;
}

unsigned char *expr_spop()
{
	if (expr_top == -1) return 0;
	else return expr_stack[expr_top--];
}

int expr_priority(unsigned char *x)
{
	unsigned char tkn_and[2];
	unsigned char tkn_or[2];
	
	tkn_and[0] = EXPR_TOKEN_AND; tkn_and[1] = 0;
	tkn_or[0] = EXPR_TOKEN_OR; tkn_or[1] = 0;

	if (strcmp((const char*)x, "(") == 0)
		return 0;
	if (strcmp((const char*)x, (const char*)tkn_and) == 0 || strcmp((const char*)x, (const char*)tkn_or) == 0)
		return 1;
	if (strcmp((const char*)x, "=") == 0 || strcmp((const char*)x, ">") == 0 || strcmp((const char*)x, "<") == 0 || strcmp((const char*)x, "<=") == 0 || strcmp((const char*)x, ">=") == 0)
		return 2;
	if (strcmp((const char*)x, "+") == 0 || strcmp((const char*)x, "-") == 0)
		return 3;
	if (strcmp((const char*)x, "*") == 0 || strcmp((const char*)x, "/") == 0)
		return 4;
	if (*x == EXPR_TOKEN_ABS)
		return 5;
}

int expr_infix_to_postfix(unsigned char *exp, unsigned char*postfix)
{
	unsigned char tkn[20];
	unsigned char *p;
	unsigned char *e;
	int ptr = 0;
	int ctr = 0;
	int num = 0;

	unsigned char exp2[160];
	e = exp;

	// eat spaces, and fix unary
	while (*e != '\0')
	{
		if (*e == 175)
			*e = EXPR_TOKEN_AND;
		if (*e == 176)
			*e = EXPR_TOKEN_OR;
		if (*e == 182)
			*e = EXPR_TOKEN_ABS;
		
		if (*e != ' ')
		{
			if (ctr == 0)
			{
				// If + or - is the first op in the expression then its a unary operator
				if (*e == '-' || *e == '+')
				{
					if (*e == '-') exp2[ctr++] = '~';
				}
				else
					exp2[ctr++] = *e;
			}
			else
				if (*e == '+' || *e == '-')
				{
					// If a + or - comes after operator, or comes after a left parenthesis, then it's an unary operator.
					if (exp2[ctr - 1] == '+' || exp2[ctr - 1] == '-' || exp2[ctr - 1] == '*' || exp2[ctr - 1] == '/' || exp2[ctr - 1] == '(')
					{
						if (*e == '-') exp2[ctr++] = '~';
					}
					else
						exp2[ctr++] = *e;
				}
				else
					exp2[ctr++] = *e;
		}
		e++;
	}
	
	exp2[ctr] = 0;
	e = exp2;

	while (*e != '\0')
	{
		if (expr_isalnum(*e))
		{
			while (expr_isalnum(*e))
			{
				if (*e == '~' && *(e + 1) == '(')
				{
					postfix[ptr++] = '0';

					tkn[0] = '-';
					tkn[1] = 0;
					expr_spush(tkn);
					e++;
					break;
				}
				postfix[ptr++] = *(e++);
			}

			postfix[ptr++] = ',';
			e--;
		}
		else if (*e == '(')
		{
			tkn[0] = *e;
			tkn[1] = 0;
			expr_spush(tkn);
		}
		else if (*e == ')')
		{
			while (1)
			{
				p = expr_spop();

				if (p == 0)
				{
					return EXPR_ERR_SYNTAX;
				}

				if (strcmp((const char*)p, "(") != 0)
				{
					while (*p != 0)
						postfix[ptr++] = *(p++);

					postfix[ptr++] = ',';
				}
				else
					break;
			}
		}
		else
		{
			if (*e == '>' && *(e + 1) == '=')
			{
				tkn[0] = '>'; tkn[1] = '=';  tkn[2] = 0; e++;
			}
			else
			if (*e == '<' && *(e + 1) == '=')
			{
				tkn[0] = '<'; tkn[1] = '=';  tkn[2] = 0; e++;
			}
			else
			{
				tkn[0] = *e;
				tkn[1] = 0;
			}
//term_printf("\n%d >= %d", expr_priority(expr_stack[expr_top]), expr_priority(tkn));
			while (expr_priority(expr_stack[expr_top]) >= expr_priority(tkn))
			{
				
				p = expr_spop();
				if (p == 0)
					break;

				while (*p != 0)
					postfix[ptr++] = *(p++);

				postfix[ptr++] = ',';
				tkn[0] = *e;
				tkn[1] = 0;
			}
			expr_spush(tkn);
		}
		e++;
	}
	while (expr_top != -1)
	{
		p = expr_spop();
		while (*p != 0)
			postfix[ptr++] = *(p++);
	}

	postfix[ptr] = 0;

	return EXPR_ERR_NONE;
}

int expr_eval_postfix(unsigned char* postfix, double* result)
{
	unsigned char tkn[20];
	unsigned char sn1[10];
	unsigned char sn2[10];
	unsigned char sn3[10];

	double n1 = 0;
	double n2 = 0;
	double n3 = 0;
	int ctr = 0, ptr=0;
	unsigned char *p, *e;
//term_printf("\n\n%s,%d", postfix, strlen((const char *)postfix));
	e = postfix;
	
	while (*e != '\0')
	{
		ctr = 0;
		if ((*e > 47 && *e < 58) || *e == '~' || *e == '.')
		{
			while ((*e > 47 && *e < 58) || *e == '~' || *e == '.')
			{
				if (*e == '~') 
					*e = '-';
				tkn[ctr++] = *(e++);
			}
			tkn[ctr] = 0;
			expr_spush(tkn);
		}

		if (*e == ',')
		{
			e++;
			continue;
		}

		p = expr_spop();
		if (p == 0)
		{
			return EXPR_ERR_SYNTAX;
		}

		ptr = 0;
		while (*p != 0)	sn1[ptr++] = *(p++);
		sn1[ptr] = 0;

		// functions
		if (*e == EXPR_TOKEN_ABS)
		{
			n3 = abs((int)atof((const char*)sn1));
		}
		else
		{
			p = expr_spop();
			if (p == 0)
			{
				return EXPR_ERR_SYNTAX;
			}
			ptr = 0;
			while (*p != 0)	sn2[ptr++] = *(p++);
			sn2[ptr] = 0;

			if(*e=='>' && *(e+1) =='=')
			{
				n3 = -(atof((const char*)sn2) >= atof((const char*)sn1));
				e++;
			}
			else
			if (*e == '<' && *(e + 1) == '=')
			{
				n3 = -(atof((const char*)sn2) <= atof((const char*)sn1));
				e++;
			}
			else
			{
				switch (*e)
				{
					case EXPR_TOKEN_AND: { n3 = -(atof((const char*)sn2) && atof((const char*)sn1)); break; }
					case EXPR_TOKEN_OR: { n3 = -(atof((const char*)sn2) || atof((const char*)sn1)); break; }
					case '+': { n3 = atof((const char*)sn2) + atof((const char*)sn1); break; }
					case '-': { n3 = atof((const char*)sn2) - atof((const char*)sn1); break; }
					case '*': { n3 = atof((const char*)sn2) * atof((const char*)sn1); break; }
					case '/':
					{
						if (atof((const char*)sn1) == 0)
						{
							return EXPR_ERR_DIV_BY_ZERO;
						}
						n3 = atof((const char*)sn2) / atof((const char*)sn1);
						break;
					}
					case '=': { n3 = -(atof((const char*)sn2) == atof((const char*)sn1));	break; }
					case '>': {	n3 = -(atof((const char*)sn2) > atof((const char*)sn1));	break; }
					case '<': { n3 = -(atof((const char*)sn2) < atof((const char*)sn1));	break; }
				}
			}
		}
		snprintf((char*)sn3, 10, "%f", n3);
		expr_spush(sn3);
		e++;
	}
	
	p = expr_spop();
	*result = atof((const char*)p);
	return EXPR_ERR_NONE;
	
}

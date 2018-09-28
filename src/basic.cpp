#include "terminal.h"
#include "basic.h"
#include "ff.h"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "vga.h"
#include "linkedlist.h"
#include "expr.h"
}

#define _BUILD_NUM_ "0.0.5"

void basic_main()
{
	term_printf("\n                 The Raspberry Pi BASIC Development System");
	term_printf("\n\n                       Operating System Version %s", _BUILD_NUM_);
	term_printf("\n\nReady.\n");

	struct Context ctx;
	unsigned char linebuf[LINE_SZ];
	int linenum, x;
	int dataPtr = 0;

	clear((unsigned char *)&ctx, sizeof(ctx));
	exec_init(&ctx);

	while (1)
	{
		get_input(linebuf);
		to_uppercase(linebuf);
		/* process line of code */
		if (ISDIGIT(linebuf[0]))
		{
			// get the full line number
			dataPtr = get_int(linebuf, 0, &linenum);
			while(linebuf[dataPtr] == ' ') dataPtr++;
			memcpy(linebuf, linebuf + dataPtr, length(linebuf) + dataPtr);

			// did user enter empty line number?
			if (isemptyline(linebuf))
				ll_delete(linenum);
			else
			{
				struct node* nodeLine = ll_find(linenum);
				unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * length(linebuf)+1);
				memcpy(data, linebuf, length(linebuf)+1);

				// If the line doesnt exist, just add it.
				// otherwise free the previous data memory and add the new line
				if (nodeLine == NULL)
					ll_insertFirst(linenum, data);
				else
				{
					free(nodeLine->data);
					nodeLine->data = data;
				}	
			}

			continue;
		}

		/* process interpreter command */
		if (ISALPHA(linebuf[0]))
		{
			if (compare(linebuf, (unsigned char*)"RUN"))
			{
				exec_program(&ctx);
			}
			else
				if (compare(linebuf, (unsigned char*)"LIST"))
				{
					
					exec_cmd_list();
				}
			else
			{
				term_printf("\n");
				if (compare(linebuf, (unsigned char*)"NEW"))
				{
					exec_cmd_new(&ctx);
				}	
				else
					if(compare(linebuf, (unsigned char*)"DIR"))
					{
						exec_cmd_dir();
					}
				else
					if(strncmp((char*)linebuf, "LOAD", 4) == 0)
					{
						int t = 0;
						while(linebuf[t] != 0)
						{
							linebuf[0+t] = linebuf[4+t];
							t++;
						}
						exec_cmd_load(linebuf);
					}	
				else
					if(strncmp((char *)linebuf, "SAVE", 4) == 0)
					{
						int t = 0;
						while(linebuf[t] != 0)
						{
							linebuf[0+t] = linebuf[4+t];
							t++;
						}
						exec_cmd_save(linebuf);
					}
					else
					{
						term_printf("?Syntax Error");
					}
			}
			term_printf("\nReady.\n");
		}
	}
}

void exec_init(struct Context *ctx)
{
	BINDCMD(&ctx->cmds[0], "PRINT", exec_cmd_print, TOKEN_PRINT);
	BINDCMD(&ctx->cmds[1], "INPUT", exec_cmd_input, TOKEN_INPUT);
	BINDCMD(&ctx->cmds[2], "RETURN", exec_cmd_return, TOKEN_RETURN);
	BINDCMD(&ctx->cmds[3], "GOTO", exec_cmd_goto, TOKEN_GOTO);
	BINDCMD(&ctx->cmds[4], "GOSUB", exec_cmd_gosub, TOKEN_GOSUB);
	BINDCMD(&ctx->cmds[5], "LET", exec_cmd_let, TOKEN_LET);
	BINDCMD(&ctx->cmds[6], "END", exec_cmd_end, TOKEN_END);
	BINDCMD(&ctx->cmds[7], "STOP", exec_cmd_end, TOKEN_STOP);
	BINDCMD(&ctx->cmds[8], "IF", exec_cmd_if, TOKEN_IF);
	BINDCMD(&ctx->cmds[9], "DIM", exec_cmd_dim, TOKEN_DIM);
	BINDCMD(&ctx->cmds[10], "REM", exec_cmd_rem, TOKEN_REM);
	BINDCMD(&ctx->cmds[11], "THEN", exec_cmd_then, TOKEN_THEN);
	BINDCMD(&ctx->cmds[12], "AND", exec_cmd_then, TOKEN_AND);
	BINDCMD(&ctx->cmds[13], "OR", exec_cmd_then, TOKEN_OR);
	BINDCMD(&ctx->cmds[14], "ABS", exec_cmd_then, TOKEN_ABS);
	BINDCMD(&ctx->cmds[15], "FOR", exec_cmd_for, TOKEN_FOR);
	BINDCMD(&ctx->cmds[16], "TO", exec_cmd_then, TOKEN_TO);
	BINDCMD(&ctx->cmds[17], "NEXT", exec_cmd_next, TOKEN_NEXT);
}

void exec_program(struct Context* ctx)
{
	int ci;

	ll_sort();

	struct node *currentNode = ll_gethead();

	ctx->running = true;
	ctx->jmpline = -1;
	ctx->line = 0;
	ctx->dsptr = 0;
	ctx->csptr = 0;
	ctx->allocated = 0;
	ctx->linePos = 0;
	ctx->error_line = 0;
	ctx->error = ERR_NONE;

	var_clear_all(ctx);

	while (ctx->running && currentNode != NULL)
	{
		ctx->original_line = currentNode->data;

		unsigned char* tokenized_line = (unsigned char*)malloc(sizeof(unsigned char) * 160);
		tokenize(ctx, ctx->original_line, tokenized_line);

		ctx->tokenized_line = tokenized_line;
		ctx->line = currentNode->linenum;

		exec_line(ctx);

		free(ctx->tokenized_line);
		
		char ch = term_getchar();
		
		if (ch == 27)
		{
			ctx->error = ERR_BREAK;
			ctx->error_line = ctx->line;
		}
		
		if (ctx->error != ERR_NONE)
		{
			switch (ctx->error)
			{
				case ERR_UNEXP:
					term_printf("\n?Syntax error");
					break;
				case ERR_UNDEF:
					term_printf("\n?Undef'd statement error");
					break;
				case ERR_BREAK:
					term_printf("\n?Break");
					break;
				case ERR_DIV0:
					term_printf("\n?Division by zero error");
					break;
				case ERR_RTRN_WO_GSB:
					term_printf("\n?RETURN without GOSUB error");
					break;
				case ERR_TYPE_MISMATCH:
					term_printf("\n?Type mismatch error");
					break;
				default:
					term_printf("\n?Unspecified error");
					break;
			}

			if(ctx->error_line != -1)
				term_printf(" in %d", ctx->error_line);

			ctx->error = ERR_NONE;
			ctx->running = false;
			return;
		}

		if (ctx->jmpline == -1)
		{
			currentNode = currentNode->next;
			ctx->linePos = 0;
		}	
		else
		{
			currentNode = ll_find(ctx->jmpline);
			ctx->jmpline = -1;
		}
	}

}

void exec_line(struct Context *ctx)
{
	int origLinePos;
	int j;
	unsigned char cmd[CMD_NAMESZ];
	bool foundCmd = false;

	while(true)
	{
		foundCmd = false;
		ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

		if (ctx->linePos == -1)
			break;

		origLinePos = ctx->linePos;
		ctx->linePos = get_symbol(ctx->tokenized_line, ctx->linePos, cmd);

		for (j = 0; j < CMD_COUNT; j++)
		{
			if (cmd[0] == ctx->cmds[j].token)
			{
				foundCmd = true;
				// execute the statement
				ctx->cmds[j].func(ctx);
				break;
			}
		}

		// Assume LET for anything else
		if (!foundCmd)
		{
			ctx->linePos = origLinePos;
			exec_cmd_let(ctx);
		}

		if (ctx->error != ERR_NONE)
			return;

		if (ctx->jmpline != -1)
			break;

		if (ctx->linePos != -1)
		{
			if (ctx->tokenized_line[ctx->linePos] != TOKEN_THEN)
				ctx->linePos = next_statement(ctx);
			else
				ctx->linePos++;
		}
	}
}

int exec_strexpr(struct Context *ctx, int* len)
{
	unsigned char *exp;
	unsigned char name[6];
	char str_value[10];
	int ctr = 0;
	int lpos = ctx->linePos;
	int expr_error = EXPR_ERR_NONE;
	double result = 0;
	unsigned char *sresult;
	unsigned char varFound = 0;
	int quoteMode = 0;

	unsigned char *str1 = NULL;

	exp = (unsigned char*)malloc(sizeof(unsigned char*) * 255);

	while (true)
	{
		if (quoteMode == 0)
		{
			lpos = ignore_space(ctx->tokenized_line, lpos);
			if (lpos == -1 || ensure_token(ctx->tokenized_line[lpos], 3, ':', ',', ';'))
				break;
		}

		// check if variable
		if (ISALPHA(ctx->tokenized_line[lpos]))
		{
			lpos = get_symbol(ctx->tokenized_line, lpos, name);

			for (int j = 0; j < ctx->var_count; j++)
			{
				if (compare(ctx->vars[j].name, name))
				{
					if (ctx->vars[j].type == VAR_INT || ctx->vars[j].type == VAR_FLOAT)
					{
						expr_error = ERR_TYPE_MISMATCH;
						break;
					}
					else
					if (ctx->vars[j].type == VAR_STRING)
					{
						unsigned char *value = (unsigned char*)ctx->vars[j].location;
						exp[ctr] = 0;
						strcat((char*)exp, (char*)value);
						ctr = strlen((const char*)exp);
						varFound = 1;
						break;
					}
				}
			}

			if (varFound == 0)
			{
				strcat((char*)exp, "");
				exp[ctr] = 0;
				var_add_update_string(ctx, name, (exp + ctr), 1);
			}

			lpos = ignore_space(ctx->tokenized_line, lpos);

			if (lpos == -1 || ensure_token(ctx->tokenized_line[lpos], 7, ':', ',', ';', '=', '<', '>', TOKEN_THEN))
				break;

			// expect plus sign
			if (ensure_token(ctx->tokenized_line[lpos], 1, '+'))
			{
				lpos++;
				lpos = ignore_space(ctx->tokenized_line, lpos);
			}
			else
			{
				ctx->error = ERR_UNEXP;
				ctx->error_line = ctx->line;
				break;
			}
		}
		else
		{
			if (ctx->tokenized_line[lpos] == '\"' && quoteMode == 0)
			{
				quoteMode = 1;
				lpos++;
			}
			else
			if (quoteMode == 1)
			{
				if (ctx->tokenized_line[lpos] == '\"')
				{
					quoteMode = 0;
					lpos++;

					lpos = ignore_space(ctx->tokenized_line, lpos);

					if (lpos == -1 || ensure_token(ctx->tokenized_line[lpos], 7, ':', ',', ';','=','<', '>', TOKEN_THEN))
						break;

					// expect plus sign
					if (ensure_token(ctx->tokenized_line[lpos], 1, '+'))
						lpos++;
					else
					{
						ctx->error = ERR_UNEXP;
						ctx->error_line = ctx->line;
						break;
					}
				}
				else
					exp[ctr++] = ctx->tokenized_line[lpos++];
			}
			else
			{
				ctx->error = ERR_UNEXP;
				ctx->error_line = ctx->line;
				break;
			}
		}	
	}

	exp[ctr] = 0;

	if(ctx->error == EXPR_ERR_NONE)
	{
		*len = strlen((char*)exp);
		ctx->strPtr = exp;
	}

	ctx->linePos = lpos;
	return lpos;
}

int exec_expr(struct Context *ctx)
{
	unsigned char exp[200];
	unsigned char name[6];
	char str_value[10];
	int ctr = 0;
	int lpos = ctx->linePos;
	int expr_error = EXPR_ERR_NONE;
	double result = 0;
	unsigned char *sresult;
	unsigned char varFound = 0;

	// replace any variables with their values before actually calling the expression parser
	while (lpos != -1 && ctx->tokenized_line[lpos] != 0 && ctx->tokenized_line[lpos] != ':' && ctx->tokenized_line[lpos] != ';' &&	ctx->tokenized_line[lpos] != ',' &&
		(ctx->tokenized_line[lpos] < 127 || ctx->tokenized_line[lpos] == TOKEN_AND || ctx->tokenized_line[lpos] == TOKEN_OR  || ctx->tokenized_line[lpos] == TOKEN_ABS))
	{
		if (ISALPHA(ctx->tokenized_line[lpos]))
		{
			lpos = get_symbol(ctx->tokenized_line, lpos, name);

			for (int j = 0; j < ctx->var_count; j++)
			{
				if (compare(ctx->vars[j].name, name))
				{
					if (ctx->vars[j].type == VAR_STRING)
					{
						expr_error = ERR_TYPE_MISMATCH;
						break;
					}
					if (ctx->vars[j].type == VAR_FLOAT)
					{
						double value = *((float*)ctx->vars[j].location);
						snprintf(str_value, 10, "%f", value);
					}
					if (ctx->vars[j].type == VAR_INT)
					{
						int value = *((int*)ctx->vars[j].location);
						snprintf(str_value, 10, "%d", value);
					}

					exp[ctr] = 0;
					strcat((char*)exp, str_value);
					ctr = strlen((const char*)exp);
					varFound = 1;
					break;
				}
			}

			if (varFound == 0)
			{
				if (name[length(name) - 1] == '$')
				{
					expr_error = ERR_TYPE_MISMATCH;
					break;
				}
				else
				if (name[length(name) - 1] == '%')
					var_add_update_int(ctx, name, ctx->dstack[ctx->dsptr--]);
				else
					var_add_update_float(ctx, name, ctx->dstack[ctx->dsptr--]);
				
				double value = 0;

				snprintf(str_value, 10, "%f", value);
				exp[ctr] = 0;
				strcat((char*)exp, str_value);
				ctr = strlen((const char*)exp);
			}
		}
		else
			exp[ctr++] = ctx->tokenized_line[lpos++];
	}

	exp[ctr] = 0;

	if(expr_error == EXPR_ERR_NONE)
		expr_error = expr_eval(exp, &result);

	switch (expr_error)
	{
		case EXPR_ERR_NONE:
		{
			ctx->dstack[++ctx->dsptr] = result;
			break;
		}
		case EXPR_ERR_DIV_BY_ZERO:
		{
			ctx->error = ERR_DIV0;
			ctx->error_line = ctx->line;
			break;
		}
		case ERR_TYPE_MISMATCH:
		{
			ctx->error = ERR_TYPE_MISMATCH;
			ctx->error_line = ctx->line;
			break;
		}
		default:
		{
			ctx->error = ERR_UNEXP;
			ctx->error_line = ctx->line;
			break;
		}
	}

	ctx->linePos = lpos;
	return lpos;


}

void exec_cmd_dim(struct Context *ctx)
{
	unsigned char name[VAR_NAMESZ];

	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);
	ctx->linePos = get_symbol(ctx->tokenized_line, ctx->linePos, name);
	exec_expr(ctx);
	//var_add_update(ctx, name, 0, ctx->dstack[ctx->dsptr--], 0);
}

void exec_cmd_dir()
{
	term_printf("\nFiles:\n");
	
	DIR dir;
	FILINFO filImage;
	FRESULT res;
	char* ext;
	
	res = f_opendir(&dir, ".");
	if (res == FR_OK)
	{
		do 
		{
			res = f_readdir(&dir, &filImage);
			ext = strrchr(filImage.fname, '.');
			if (res == FR_OK && filImage.fname[0] != 0)
			{
				term_printf("%s\n",filImage.fname);
			}
		}
		while (res == FR_OK && filImage.fname[0] != 0);
		
		f_closedir(&dir);
	}
	else
		term_printf("\n?Disk read error.");

}

void exec_cmd_end(struct Context *ctx)
{
	ctx->running = false;
}

void exec_cmd_for(struct Context *ctx)
{
	double startval = 0;
	double endval = 0;
	double stepval = 0;
	TokenType tkn;
	unsigned char varname[5] = { 0 };
	

	tkn.token = (unsigned char*)malloc(sizeof(unsigned char) * 160);

	ctx->linePos = get_token(ctx->tokenized_line, ctx->linePos, &tkn);
	if (tkn.type == TOKEN_TYPE_FLT_VAR || tkn.type == TOKEN_TYPE_INT_VAR)
	{
		strcpy((char*)varname, (char*)tkn.token);

		ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

		if (ensure_token(ctx->tokenized_line[ctx->linePos], 1, '='))
		{
			ctx->linePos = ignore_space(ctx->tokenized_line, ++ctx->linePos);
			ctx->linePos = exec_expr(ctx);
			startval = ctx->dstack[ctx->dsptr--];

			if (varname[length(varname) - 1] == '%')
				var_add_update_int(ctx, varname, startval);
			else
				var_add_update_float(ctx, varname, startval);

			ctx->linePos = get_token(ctx->tokenized_line, ctx->linePos, &tkn);
			if (tkn.type == TOKEN_TYPE_KEYWORD && tkn.token[0] == TOKEN_TO)
			{
				//ctx->linePos = ignore_space(ctx->tokenized_line, ++ctx->linePos);
				ctx->linePos = exec_expr(ctx);

				endval = ctx->dstack[ctx->dsptr--];

				ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

				if (ctx->linePos == -1 || ctx->tokenized_line[ctx->linePos] == ':')
				{
					struct forstackitem fsi;
					fsi.var = (unsigned char *)malloc(sizeof(unsigned char)* length(varname) + 1);
					strcpy((char *)fsi.var, (char *)varname);
					fsi.endval = endval;
					fsi.linenumber = ctx->line;
					fsi.linepos = ctx->linePos-1;
					fsi.step = 1;

					forstack[forstackidx++] = fsi;
				}
				else
				{
					ctx->error = ERR_UNEXP;
					ctx->error_line = ctx->line;
				}
			}
			else
			{
				ctx->error = ERR_UNEXP;
				ctx->error_line = ctx->line;
			}

		}
		else
		{
			ctx->error = ERR_UNEXP;
			ctx->error_line = ctx->line;
		}

	}
	else
	{
		ctx->error = ERR_UNEXP;
		ctx->error_line = ctx->line;
	}

	free(tkn.token);
}

void exec_cmd_gosub(struct Context *ctx)
{
	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

	if (ensure_token(ctx->tokenized_line[ctx->linePos], DIGIT_LIST_COUNT, DIGIT_LIST))
	{
		exec_expr(ctx);

		// store line number and next statement pos on stack
		ctx->linePos = next_statement(ctx);

		ctx->cstack[++ctx->csptr] = ctx->line;
		ctx->cstack[++ctx->csptr] = ctx->linePos;

		// set jump flag
		ctx->jmpline = ctx->dstack[ctx->dsptr--];
		ctx->linePos = 0;

		if (ll_find(ctx->jmpline) == NULL)
		{
			ctx->error = ERR_UNDEF;
			ctx->error_line = ctx->line;
			return;
		}
	}
	else
	{
		ctx->error = ERR_UNEXP;
		ctx->error_line = ctx->line;
	}
}

void exec_cmd_goto(struct Context *ctx)
{
	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

	if (ensure_token(ctx->tokenized_line[ctx->linePos], DIGIT_LIST_COUNT, DIGIT_LIST))
	{
		exec_expr(ctx);

		ctx->linePos = 0;
		ctx->jmpline = ctx->dstack[ctx->dsptr--];
		
		if (ll_find(ctx->jmpline) == NULL)
		{
			ctx->error = ERR_UNDEF;
			ctx->error_line = ctx->line;
			return;
		}
	}
	else
	{
		ctx->error = ERR_UNEXP;
		ctx->error_line = ctx->line;
	}
}

void exec_cmd_if(struct Context *ctx)
{
	TokenType tkn;
	tkn.token = (unsigned char*)malloc(sizeof(unsigned char*) * 160);
	get_token(ctx->tokenized_line, ctx->linePos, &tkn);

	int len;
	int result1 = -1;
	int result2 = 0;

	if (tkn.type == TOKEN_TYPE_STR_LIT || tkn.type == TOKEN_TYPE_STR_VAR || tkn.type == TOKEN_TYPE_STR_FUNC)
	{
		ctx->linePos = exec_strexpr(ctx, &len);

		get_token(ctx->tokenized_line, ctx->linePos, &tkn);
		if (tkn.type == TOKEN_TYPE_EQUALITY)
		{
			if (strcmp((char *)tkn.token, "=") == 0)
				ctx->linePos++;
			else				
			if (strcmp((char *)tkn.token, "<>") == 0)
			{
				result1 = 0;
				result2 = -1;
				ctx->linePos += 2;
			}
			else
			{
				ctx->error = ERR_UNEXP;
				ctx->error_line = ctx->line;
			}

			unsigned char *str1 = (unsigned char *)malloc(sizeof(unsigned char) * len);
			str1 = ctx->strPtr;

			TokenType tkn2;
			tkn2.token = (unsigned char*)malloc(sizeof(unsigned char*) * 160);
			get_token(ctx->tokenized_line, ctx->linePos, &tkn2);

			if (tkn2.type == TOKEN_TYPE_STR_LIT || tkn2.type == TOKEN_TYPE_STR_VAR || tkn2.type == TOKEN_TYPE_STR_FUNC)
			{
				ctx->linePos = exec_strexpr(ctx, &len);

				if (strcmp((char *)ctx->strPtr, (char *)str1) == 0)
					ctx->dstack[++ctx->dsptr] = result1;
				else
					ctx->dstack[++ctx->dsptr] = result2;
			}
			else
			{
				ctx->error = ERR_UNEXP;
				ctx->error_line = ctx->line;
			}

			free(str1);
			free(tkn2.token);
		}
	}
	else
	if (tkn.type == TOKEN_TYPE_INT_LIT || tkn.type == TOKEN_TYPE_INT_VAR || tkn.type == TOKEN_TYPE_INT_FUNC ||
		tkn.type == TOKEN_TYPE_FLT_LIT || tkn.type == TOKEN_TYPE_FLT_VAR || tkn.type == TOKEN_TYPE_FLT_FUNC || tkn.type == TOKEN_TYPE_OPERATOR)
		ctx->linePos = exec_expr(ctx);

	free(tkn.token);

	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

	if (ensure_token(ctx->tokenized_line[ctx->linePos], 1, TOKEN_THEN))
	{
		if (ctx->dstack[ctx->dsptr--] == -1)
			return;
		else
			ctx->linePos = -1;
	}
	else
	{
		ctx->error = ERR_UNEXP;
		ctx->error_line = ctx->line;
	}
}

void exec_cmd_input(struct Context *ctx)
{
	unsigned char name[VAR_NAMESZ+3], ch;
	float value = 0;
	int j;
	int promptCtr = 0;
	unsigned char prompt[160] = { 0 };

	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

	if (ctx->linePos == -1 || ensure_token(ctx->tokenized_line[ctx->linePos], 1, ':'))
	{
		ctx->error = ERR_UNEXP;
		ctx->error_line = ctx->line;
		return;
	}
	
	if (ctx->tokenized_line[ctx->linePos] == '\"')
	{
		ctx->linePos++;

		while (ctx->tokenized_line[ctx->linePos] != '\"')
			prompt[promptCtr++] = ctx->tokenized_line[ctx->linePos++];

		prompt[promptCtr] = 0;

		/* ignore quotation mark */
		ctx->linePos++;

		/* ensure semicolon */
		if (ensure_token(ctx->tokenized_line[ctx->linePos], 1, ';'))
		{
			term_printf("%s", prompt);
			ctx->linePos++;
		}
		else
		{
			ctx->error = ERR_UNEXP;
			ctx->error_line = ctx->line;
			return;
		}
	}

	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);
	ctx->linePos = get_symbol(ctx->tokenized_line, ctx->linePos, name);

	ch = 0;

	unsigned char buffer[160] = { 0 };
	int bufferctr = 0;

	term_printf("? ");
	
	while (true)
	{
		char ch = term_getchar();

		if (ch != 0)
		{
			term_putchar(ch);

			if (ch == 27)
			{
				ctx->error = ERR_BREAK;
				ctx->error_line = ctx->line;
				return;
			}

			if (ch == '\n')
			{
				buffer[bufferctr] = '\0';
				break;
			}
			else if (ch == 8)
			{
				if (bufferctr > 0)
					bufferctr--;
			}
			else
				buffer[bufferctr++] = ch;
		}
	}

	if (name[length(name) - 1] == '$')
	{
		var_add_update_string(ctx, name, buffer, length(buffer));
	}
	else
	{
		get_float(buffer, 0, &value);
		var_add_update_float(ctx, name, value);
	}
	
	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);
	
	// anything after expression must be end of line or colon
	if (ctx->linePos != -1 && ctx->tokenized_line[ctx->linePos] != ':')
	{
		ctx->error = ERR_UNEXP;
		ctx->error_line = ctx->line;
		return;
	}
}

void exec_cmd_let(struct Context *ctx)
{
	unsigned char name[VAR_NAMESZ+2]; // 2 chars, var type, \0
	int j = 0;
	int len = 0;

	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);
	ctx->linePos = get_symbol(ctx->tokenized_line, ctx->linePos, name);
	ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

	if (ctx->tokenized_line[ctx->linePos] == '[')
	{
		ctx->linePos = exec_expr(ctx);
		ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);
		j = ctx->dstack[ctx->dsptr--];
	}

	// anything after token must be equal sign
	if (ensure_token(ctx->tokenized_line[ctx->linePos], 1, '='))
	{
		ctx->linePos++;

		if (name[length(name) - 1] == '$')
			exec_strexpr(ctx, &len);
		else
			exec_expr(ctx);
		
		ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

		// anything after expression must be end of line or colon
		if (ctx->linePos != -1 && ctx->tokenized_line[ctx->linePos] != ':')
		{
			ctx->error = ERR_UNEXP;
			ctx->error_line = ctx->line;
			return;
		}

		if (name[length(name) - 1] == '%')
			var_add_update_int(ctx, name, ctx->dstack[ctx->dsptr--]);
		else if (name[length(name) - 1] == '$')
			var_add_update_string(ctx, name, ctx->strPtr, len);
		else
			var_add_update_float(ctx, name, ctx->dstack[ctx->dsptr--]);
	}
	else
	{
		ctx->error = ERR_UNEXP;
		ctx->error_line = ctx->line;
		return;
	}
}

void exec_cmd_list()
{
	ll_sort();

	struct node *ptr = ll_gethead();
	term_printf("\n");

	//start from the beginning
	while (ptr != NULL)
	{
		char lnbuff[10] = { 0 };
		snprintf(lnbuff, sizeof(lnbuff), "%d", ptr->linenum);
		term_printf("%s %s\n", lnbuff, ptr->data);
		ptr = ptr->next;
	}
}

void exec_cmd_load(unsigned char* filename)
{
	FIL fp;
	FRESULT res;
	UINT bytesRead;
	BYTE b[2];
	char buffer[160];
	int bufferCtr = 0;
	int linenum = 0;
	int dataPtr = 0;
	char fnbuffer[20] = {0};
	
	// skip spaces
	int t=0;
	while(filename[t] == ' ') t++;
	
	// expect quote
	if(filename[t] != '\"')
	{
		term_printf("?Syntax error");
		return;
	}
	
	t++;
	while(filename[t] != 0 && filename[t] != '\"')
	{
		fnbuffer[bufferCtr++] = filename[t++];
	
		if(bufferCtr == 15)
			filename[t] = '\"';
	}
	
	
	// expect closing quote
	if(filename[t] != '\"')
	{
		term_printf("?Syntax error");
		return;
	}
	
	fnbuffer[bufferCtr++] = '.';
	fnbuffer[bufferCtr++] = 'B';
	fnbuffer[bufferCtr++] = 'A';
	fnbuffer[bufferCtr++] = 'S';
	fnbuffer[bufferCtr] = 0;
	
	to_uppercase((unsigned char *)fnbuffer);
	
	bufferCtr = 0;
	
	// new cmd
	while(!ll_isEmpty())
		ll_deleteFirst();

	//snprintf(fnbuffer, sizeof(fnbuffer), "%s.BAS", filename);
	
	term_printf("Searching for %s\n", fnbuffer);
	
	res = f_open(&fp, fnbuffer, FA_READ | FA_OPEN_EXISTING);
	
	if (res == FR_OK)
	{
		term_printf("Loading");
		
		while (true) 
		{
			res = f_read(&fp, b, 1, &bytesRead);
			
			if(res == FR_OK && bytesRead == 1)
			{
				if(b[0] != '\n')
					buffer[bufferCtr++] = b[0];
				else 
				{
					if(bufferCtr != 0)
					{
						buffer[bufferCtr] = 0;
						dataPtr = get_int((unsigned char *)buffer, 0, &linenum);
						dataPtr = ignore_space((unsigned char *)buffer, dataPtr);
						memcpy(buffer, buffer + dataPtr, strlen(buffer)+dataPtr);						
						char *data = (char *)malloc(sizeof(char) * strlen(buffer) + 1);
						memcpy(data, buffer, strlen(buffer) + 1);
						ll_insertFirst(linenum, (unsigned char *)data);
						bufferCtr=0;
					}
				}
			}
			else
			{
				if(bufferCtr != 0)
				{
					buffer[bufferCtr] = 0;
					dataPtr = get_int((unsigned char *)buffer, 0, &linenum);
					dataPtr = ignore_space((unsigned char *)buffer, dataPtr);
					memcpy(buffer, buffer + dataPtr, strlen(buffer)+dataPtr);
					char *data = (char *)malloc(sizeof(char) * strlen(buffer) + 1);
					memcpy(data, buffer, strlen(buffer) + 1);
					ll_insertFirst(linenum, (unsigned char *)data);
					bufferCtr=0;
				}
				break;
			}
		};

		ll_sort();
		f_close(&fp);
	}
	else
		term_printf("?File load error #%d", res);
}

void exec_cmd_new(struct Context *ctx)
{
	while (!ll_isEmpty())
		ll_deleteFirst();

	var_clear_all(ctx);
}

void exec_cmd_next(struct Context *ctx)
{
	TokenType tkn;
	double currentvalue = 0;
	unsigned char varname[10];
	bool found = false;

	tkn.token = (unsigned char*)malloc(sizeof(unsigned char) * 160);

	ctx->linePos = get_token(ctx->tokenized_line, ctx->linePos, &tkn);
	if (tkn.type == TOKEN_TYPE_FLT_VAR || tkn.type == TOKEN_TYPE_INT_VAR)
	{
		ctx->linePos = ctx->linePos - length(tkn.token);
		ctx->linePos = exec_expr(ctx);
		currentvalue = ctx->dstack[ctx->dsptr--];

		for (int x = 0; x < forstackidx; x++)
		{
			if (strcmp((char *)forstack[x].var, (char *)tkn.token) == 0)
			{
				found = true;
				var_add_update_float(ctx, tkn.token, currentvalue + forstack[x].step);

				if (currentvalue + forstack[x].step <= forstack[x].endval)
				{
					// Get previous line pos and line number
					ctx->linePos = forstack[x].linepos;
					ctx->line = ll_find(forstack[x].linenumber)->linenum;
					ctx->original_line = ll_find(forstack[x].linenumber)->data;

					// retokenize the line
					unsigned char* tokenized_line = (unsigned char*)malloc(sizeof(unsigned char) * 160);
					tokenize(ctx, ctx->original_line, tokenized_line);

					ctx->tokenized_line = tokenized_line;
					ctx->jmpline = ctx->line;
					break;
				}
				else
				{
					forstackidx--;

					ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

					if (ctx->linePos != -1 && ctx->tokenized_line[ctx->linePos] != ':')
					{
						ctx->error = ERR_UNEXP;
						ctx->error_line = ctx->line;
						break;
					}
				}

			}
		}

		if (found == false)
		{
			// next without for
			ctx->error = ERR_UNEXP;
			ctx->error_line = ctx->line;
		}

	}
	else
	{
		ctx->error = ERR_UNEXP;
		ctx->error_line = ctx->line;
	}
}

void exec_cmd_print(struct Context *ctx)
{
	bool eol = true;
	bool quoteMode = false;
	unsigned char val[200];

	while (true)
	{
		ctx->linePos = ignore_space(ctx->tokenized_line, ctx->linePos);

		if (ctx->linePos == -1 || ensure_token(ctx->tokenized_line[ctx->linePos], 1, ':'))
			break;

		ctx->linePos = get_symbol(ctx->tokenized_line, ctx->linePos, val);

		// print string expression
		if (val[length(val) - 1] == '$' || val[0] == '\"')
		{
			ctx->linePos = ctx->linePos - length(val);
			int len = 0;
			ctx->linePos = exec_strexpr(ctx, &len);
			if (ctx->error == ERR_NONE)
			{
				term_printf("%s", ctx->strPtr);
				free(ctx->strPtr);
			}	
			else
				break;
		}
		else
		{
			// print numeric expression
			ctx->linePos = ctx->linePos - length(val);
			ctx->linePos = exec_expr(ctx);
			if (ctx->error == ERR_NONE)
				term_printf("%g", (double)ctx->dstack[ctx->dsptr--]);
			else
				break;
		}

		if (ctx->tokenized_line[ctx->linePos] == ';')
		{
			ctx->linePos++;
			eol = false;
		}

		if (ctx->tokenized_line[ctx->linePos] == ',')
		{
			ctx->linePos++;
			term_printf("     ");
		}
	}

	if (eol) term_putchar('\n');
}

void exec_cmd_rem(struct Context *ctx)
{
	// skip this line (do nothing)
	return;
}

void exec_cmd_return(struct Context *ctx)
{
	if (ctx->csptr == 0)
	{
		ctx->error = ERR_RTRN_WO_GSB;
		ctx->error_line = ctx->line;
		return;
	}

	// release current line resource
	free(ctx->tokenized_line);
	
	// Get previous line pos and line number from stack
	ctx->linePos = ctx->cstack[ctx->csptr--];
	ctx->line = ll_find(ctx->cstack[ctx->csptr])->linenum;
	ctx->original_line = ll_find(ctx->cstack[ctx->csptr--])->data;
	
	// retokenize the line
	unsigned char* tokenized_line = (unsigned char*)malloc(sizeof(unsigned char)*160);
	tokenize(ctx, ctx->original_line, tokenized_line);

	ctx->tokenized_line = tokenized_line;
	ctx->jmpline = ctx->line;
}

void exec_cmd_save(unsigned char* filename)
{
	FIL fp;
	FRESULT res;
	int bufferCtr = 0;
	char fnbuffer[15] = {0};

	// skip spaces
	int t=0;
	while(filename[t] == ' ') t++;
	
	// expect quote
	if(filename[t] != '\"')
	{
		term_printf("?Syntax error");
		return;
	}
	
	t++;
	while(filename[t] != 0 && filename[t] != '\"')
	{
		fnbuffer[bufferCtr++] = filename[t++];
	
		if(bufferCtr == 15)
			filename[t] = '\"';
	}
	
	// expect closing quote
	if(filename[t] != '\"')
	{
		term_printf("?Syntax error");
		return;
	}
	
	fnbuffer[bufferCtr++] = '.';
	fnbuffer[bufferCtr++] = 'B';
	fnbuffer[bufferCtr++] = 'A';
	fnbuffer[bufferCtr++] = 'S';
	fnbuffer[bufferCtr] = 0;
	
	to_uppercase((unsigned char *)fnbuffer);
	
	bufferCtr = 0;

	//snprintf(fnbuffer, sizeof(fnbuffer), "%s.BAS", filename);
	
	res = f_open(&fp, fnbuffer, FA_WRITE | FA_CREATE_NEW);
	
	if (res == FR_OK)
	{
		term_printf("Saving %s", fnbuffer);
		
		ll_sort();
		struct node *ptr = ll_gethead();

		//start from the beginning
		while (ptr != NULL) 
		{
			char lnbuff[160] = { 0 };
			snprintf(lnbuff, sizeof(lnbuff), "%d %s\n", ptr->linenum, ptr->data);
			f_puts(lnbuff, &fp);
			ptr = ptr->next;
		}
		
		f_close(&fp);
	}
	else
		term_printf("?File Save Error #%d", res);
}

void exec_cmd_then(struct Context *ctx)
{
	// skip this line (do nothing)
	return;
}

void var_clear_all(struct Context *ctx)
{
	for (int j = 0; j < ctx->var_count; j++)
	{
		ctx->vars[j].name[0] = 0;
		ctx->vars[j].type = VAR_NONE;
		free(ctx->vars[j].location);
	}

	ctx->var_count = 0;
}

void var_add_update_int(struct Context *ctx, const unsigned char *key, int value)
{
	int j;

	// find and update the variable
	for (j = 0; j < ctx->var_count; j++)
	{
		if (compare(ctx->vars[j].name, key) && ctx->vars[j].type == VAR_INT)
		{
			free(ctx->vars[j].location);
			ctx->vars[j].location = (int *)malloc(sizeof(int));
			*((int*)ctx->vars[j].location) = value;
			return;
		}
	}
	
	// variable not found.  add it
	ctx->var_count = j+1;
	strcpy((char *)ctx->vars[ctx->var_count-1].name, (char *)key);
	ctx->vars[ctx->var_count-1].location = (int *)malloc(sizeof(int));
	ctx->vars[ctx->var_count-1].type = VAR_INT;
	*((int*)ctx->vars[ctx->var_count-1].location) = value;
	
}

void var_add_update_float(struct Context *ctx, const unsigned char *key, float value)
{
	int j;

	// find and update the variable
	for (j = 0; j < ctx->var_count; j++)
	{
		if (compare(ctx->vars[j].name, key) && ctx->vars[j].type == VAR_FLOAT)
		{
			free(ctx->vars[j].location);
			ctx->vars[j].location = (float *)malloc(sizeof(float));
			*((float*)ctx->vars[j].location) = value;
			return;
		}
	}

	// variable not found.  add it
	ctx->var_count = j + 1;
	strcpy((char *)ctx->vars[ctx->var_count - 1].name, (char *)key);
	ctx->vars[ctx->var_count - 1].location = (float *)malloc(sizeof(float));
	ctx->vars[ctx->var_count - 1].type = VAR_FLOAT;
	*((float*)ctx->vars[ctx->var_count - 1].location) = value;

}

void var_add_update_string(struct Context *ctx, const unsigned char *key, unsigned char* value, int length)
{
	int j;

	// find and update the variable
	for (j = 0; j < ctx->var_count; j++)
	{
		if (compare(ctx->vars[j].name, key) && ctx->vars[j].type == VAR_STRING)
		{
			free(ctx->vars[j].location);
			ctx->vars[j].location = (unsigned char*)malloc(sizeof(unsigned char*) * length+1);

			for(int x=0;x<length;x++)
				*((char*)ctx->vars[j].location+x) = value[x];

			*((char*)ctx->vars[j].location + length) = 0;
			return;
		}
	}

	// variable not found.  add it
	ctx->var_count = j + 1;
	strcpy((char *)ctx->vars[ctx->var_count - 1].name, (char *)key);
	ctx->vars[ctx->var_count - 1].location = (unsigned char *)malloc(sizeof(unsigned char*) * length + 1);
	ctx->vars[ctx->var_count - 1].type = VAR_STRING;
	
	for (int x = 0; x<length; x++)
		*((unsigned char*)ctx->vars[j].location + x) = value[x];

	*((unsigned char*)ctx->vars[j].location + length) = 0;

}

// sbparse

int get_symbol(const unsigned char *s, int i, unsigned char *t)
{
	int done = 0;
	int type = 0;

	if (s[i] > 127) type = 0; // cmd / func
	if (ISALPHA(s[i])) type = 1; //var
	if (s[i] == '\"') 
	{
		*(t++) = s[i++];
		type = 2; // str
	}

	while (true)
	{
		if (s[i] == 0)
			break;

		if(type == 0)
		{
			if (s[i] > 127)
				*(t++) = s[i++];
			break;
		}
		else
		if (type == 1)
		{
			if (ISALPHA(s[i]))
				*(t++) = s[i++];

			// string
			if (s[i] == '$')
				*(t++) = s[i++];

			// int
			if (s[i] == '%')
				*(t++) = s[i++];

			break;
		}
		else
		if(type == 2)
		{
			if (s[i] != '\"')
				*(t++) = s[i++];
			else
			{
				*(t++) = s[i++];
				break;
			}
				
		}
	}

	*t = 0;
	return i;
}

int get_token(const unsigned char *s, int i, TokenType* token)
{
	int done = 0;
	token->type = TOKEN_TYPE_UNKNOWN;

	while (s[i] == ' ')
		i++;

	if (s[i] > 127)
	{
		int ctr = 0;
		token->type = TOKEN_TYPE_KEYWORD;
		token->token[ctr++] = s[i++];
		token->token[ctr] = 0;
		return i;
	}
	if (s[i] == '+' || s[i] == '-' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '(' || s[i] == ')')
	{
		int ctr = 0;
		token->type = TOKEN_TYPE_OPERATOR;
		token->token[ctr++] = s[i++];
		token->token[ctr] = 0;
		return i;
	}
	if (s[i] == '=' || s[i] == '>' || s[i] == '<')
	{
		int ctr = 0;
		token->type = TOKEN_TYPE_EQUALITY;
		token->token[ctr++] = s[i++];

		if ((s[i - 1] == '<' || s[i - 1] == '>') && s[i] == '=')
			token->token[ctr++] = s[i++];
		else
			if (s[i - 1] == '<' && s[i] == '>')
				token->token[ctr++] = s[i++];
		token->token[ctr] = 0;
		return i;
	}
	if (s[i] == '\"')
	{
		token->type = TOKEN_TYPE_STR_LIT;
		int ctr = 0;
		while (s[i] != 0)
		{
			token->token[ctr++] = s[i++];
			if (s[i - 1] == '\"')
				break;
		}
		token->token[ctr] = 0;
		return i;
	}
	if (ISALPHA(s[i]))
	{
		int ctr = 0;
		while (s[i] != 0 && ISALPHA(s[i]))
		{
			token->token[ctr++] = s[i++];
		}

		if (s[i] == '$')
		{
			token->type = TOKEN_TYPE_STR_VAR;
			token->token[ctr++] = s[i++];
		}
		else
		if (s[i] == '%')
		{
			token->type = TOKEN_TYPE_INT_VAR;
			token->token[ctr++] = s[i++];
		}
		else
		{
			token->type = TOKEN_TYPE_FLT_VAR;
		}
		token->token[ctr] = 0;
		return i;
	}
	if (ISDIGIT(s[i]) || s[i] == '.')
	{
		int ctr = 0;
		token->type = TOKEN_TYPE_INT_LIT;

		while (s[i] != 0 && (ISDIGIT(s[i]) || s[i] == '.'))
		{
			if (s[i] == '.')
				token->type = TOKEN_TYPE_FLT_LIT;
			token->token[ctr++] = s[i++];
		}

		token->token[ctr] = 0;
		return i;
	}
	if (s[i] == ':' || s[i] == ';' || s[i] == ',')
	{
		int ctr = 0;
		token->type = TOKEN_TYPE_DELIMITER;
		token->token[ctr++] = s[i++];
		token->token[ctr] = 0;
		return i;
	}
	if (s[i] == 0)
	{
		int ctr = 0;
		token->type = TOKEN_TYPE_EOL;
		token->token[ctr++] = s[i++];
		token->token[ctr] = 0;
		return i;
	}

	return i;
}

int get_int(const unsigned char *s, int i, int *num)
{
	*num = 0;
	while (s[i] && ISDIGIT(s[i]))
	{
		*num *= 10;
		*num += s[i++] - '0';
	}
	return i;
}

int get_float(const unsigned char* s, int i, float *fv)
{
	float rez = 0, fact = 1;

	if (i == 0 && s[i] == '-')
	{
		i++;
		fact = -1;
	};

	for (int point_seen = 0; ISDIGIT(s[i]) || (s[i] == '.' && point_seen == 0); i++)
	{
		if (s[i] == '.' && point_seen == 0)
		{
			point_seen = 1;
			continue;
		}

		int d = s[i] - '0';

		if (d >= 0 && d <= 9)
		{
			if (point_seen) fact /= 10.0f;
			rez = rez * 10.0f + (float)d;
		};
	};

	*fv = rez * fact;
	return i;
};

int ignore_space(const unsigned char *s, int i)
{
	while (s[i] && ISSPACE(s[i]))
	{
		i++;
	}

	if (s[i] == 0)
		return -1;
	else
		return i;
}

void to_uppercase(unsigned char *s)
{
	bool toggle = true;
	while (*s)
	{
		if (ISalpha(*s))
		{
			if (toggle)
				*s = *s - 32;
		}
		else
			if (*s == '\"')
			{
				toggle = !toggle;
			}
			else
				if (*s == '\'')
				{
					toggle = false;
				}
				else
					if (*s == '\n')
					{
						toggle = true;
					}
		s++;
	}
}

bool isemptyline(unsigned char *linebuf)
{
	bool emptyline = true;
	int ctr = 0;
	while (ISDIGIT(linebuf[ctr]))
	{
		ctr++;
	}

	while (linebuf[ctr] != '\0')
	{
		if (!ISSPACE(linebuf[ctr]))
		{
			emptyline = false;
			break;
		}
		ctr++;
	}

	return emptyline;
}

bool compare(const unsigned char *a, const unsigned char *b)
{
	if (a == false && b == false) return true;
	if (a == false || b == false) return false;
	while (*a && *b && (*a == *b))
	{
		a++;
		b++;
	}
	if ((*a == '\0') && (*b == '\0')) return true;
	return false;
}

void clear(unsigned char *dst, int size)
{
	for (; size>0; size--, dst++)
	{
		*dst = 0;
	}
}

int length(const unsigned char *s)
{
	int l = 0;
	while (*(s++)) l++;
	return l;
}

void join(unsigned char *dst, const unsigned char *src)
{
	strcpy((char *)(dst + strlen((char *)dst)), (char *)src);
}

void get_input(unsigned char *s)
{
	int inbufferctr = 0;

	while (true)
	{
		char c = term_getchar();

		if (c != 0)
		{
			term_putchar(c);

			if (c == '\n')
			{
				s[inbufferctr] = '\0';
				break;
			}
			else if (c == 8)
			{
				if (inbufferctr > 0)
					inbufferctr--;
			}
			else
				s[inbufferctr++] = c;
		}
	}
}

int next_statement(struct Context *ctx)
{
	int ctr = ctx->linePos;
	int quoteMode = 0;

	while (true)
	{
		if (ctx->tokenized_line[ctr] == '\0' || (ctx->tokenized_line[ctr] == ':' && quoteMode == 0))
			break;

		if (ctx->tokenized_line[ctr] == '\"')
		{
			if (quoteMode == 0)
				quoteMode = 1;
			else
				quoteMode = 0;
		}
		ctr++;
	}

	if (ctx->tokenized_line[ctr] == ':')
		ctr++;
	else
		ctr = -1;

	return ctr;
}

void tokenize(struct Context* ctx, const unsigned char* input, unsigned char *output)
{
	int i = 0, o = 0, tempStackPtr = 0;
	bool quoteMode = false;
	bool foundCmd = false;
	unsigned char tempStack[160] = { 0 };

	while (true)
	{
		if (input[i] == 0 || input[i] == ':')
		{
			if (tempStackPtr != 0)
			{
				for (int x = 0; x < tempStackPtr; x++)
					output[o++] = tempStack[x];
				tempStackPtr = 0;
			}

			output[o] = input[i];

			if (input[i] == 0)
				break;
			else
			{
				i++;
				o++;
				continue;
			}
		}

		if (quoteMode == false)
		{
			if (ISALPHA(input[i]))
			{
				// push to the stack
				tempStack[tempStackPtr++] = input[i++];
				tempStack[tempStackPtr] = 0;

				for (int j = 0; j < CMD_COUNT; j++)
				{
					if (compare(tempStack, ctx->cmds[j].name))
					{
						output[o++] = ctx->cmds[j].token;
						tempStackPtr = 0;
						break;
					}
				}
			}
			else
			{
				if (input[i] == '\"')
					quoteMode = true;

				tempStack[tempStackPtr++] = input[i++];

				for (int x = 0; x < tempStackPtr; x++)
					output[o++] = tempStack[x];
				
				tempStackPtr = 0;
			}
		}

		if (quoteMode == true)
		{
			if(input[i] == '\"')
				quoteMode = false;
			
			output[o++] = input[i++];
		}
	};

	
	to_uppercase(output);
}

bool ensure_token(unsigned char c, int tokenCount, ...)
{
	// set up variable argument list
	va_list ap;
	va_start(ap, tokenCount);

	for (int x = 0; x < tokenCount; x++)
	{
		unsigned char t = (unsigned char)va_arg(ap, int);
		if (c == t)
		{
			va_end(ap);
			return true;
		}	
	}

	va_end(ap);
	return false;
}

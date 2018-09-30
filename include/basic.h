#ifndef BASIC_H
#define BASIC_H

extern "C"
{

#define true 1
#define false 0
	//typedef char bool;

#define NO_LINE -1

#define VAR_NAMESZ 2       /* maximum variable name length */
#define CMD_NAMESZ 10       /* limit for command words */
#define CMD_COUNT 25        /* number of available commands */
#define DATA_STSZ 16        /* depth of calculation */
#define CALL_STSZ 16        /* subroutine call depth */
#define LINE_SZ 80          /* line width restriction */
#define CODE_SZ 4096        /* program size in characters */
#define FILE_SUPPORT 0   	/* 0 - no, 1 - yes */

#define ERR_NONE			0
#define ERR_UNDEF			-1
#define ERR_UNEXP			-2
#define ERR_UNKNOWN			-3
#define ERR_BREAK			-4
#define ERR_DIV0			-5
#define ERR_RTRN_WO_GSB		-6
#define ERR_TYPE_MISMATCH	-7
#define ERR_NEXT_WO_FOR		-8
#define ERR_ILLEGAL_DIRECT	-9

#define VAR_NONE	0
#define VAR_INT		1
#define VAR_FLOAT	2
#define VAR_STRING	3

#define TOKEN_TYPE_UNKNOWN		0
#define TOKEN_TYPE_KEYWORD		1
#define TOKEN_TYPE_OPERATOR		2
#define TOKEN_TYPE_EQUALITY		3
#define TOKEN_TYPE_INT_LIT		4
#define TOKEN_TYPE_FLT_LIT		5
#define TOKEN_TYPE_STR_LIT		6
#define TOKEN_TYPE_STR_VAR		7
#define TOKEN_TYPE_INT_VAR		8
#define TOKEN_TYPE_FLT_VAR		9
#define TOKEN_TYPE_INT_FUNC		10
#define TOKEN_TYPE_FLT_FUNC		11
#define TOKEN_TYPE_STR_FUNC		12
#define TOKEN_TYPE_DELIMITER	13
#define TOKEN_TYPE_EOS			14
#define TOKEN_TYPE_EOL			15

struct Token {
	int type;
	unsigned char *token;
};

typedef Token TokenType;

// for stack
struct forstackitem {
	unsigned char *var;
	int linenumber;
	int linepos;
	double step;
	double endval;
};

static struct forstackitem forstack[100];
static int forstackidx = 0;

#define DIGIT_LIST  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
#define DIGIT_LIST_COUNT 10

#define BINDCMD(c,n,m,f,t)  do {\
							strcpy((char *)((c)->name), (char *)n);\
							(c)->directmode = m;\
							(c)->func = f;\
							(c)->token = t;\
						} while(0)

struct Variable
{
	unsigned char name[VAR_NAMESZ];
	int type;
	int length;
	void* location;
};

struct Command
{
	unsigned char name[CMD_NAMESZ];
	bool directmode;
	unsigned char token;
	void(*func)(struct Context*);
};

struct Context
{
	struct Variable vars[100];
	struct Command cmds[CMD_COUNT];
	int var_count;
	float dstack[DATA_STSZ];
	int cstack[CALL_STSZ];
	int dsptr;
	int csptr;
	unsigned char *strPtr;
	int allocated;
	int line;
	int jmpline;
	int top_line;
	char error;
	int error_line;
	bool running;
	unsigned char* original_line;
	unsigned char* tokenized_line;
	int linePos;
};

void basic_main();
void exec_init(struct Context *ctx);
void exec_program(struct Context* ctx);
void exec_line(struct Context *ctx);
int exec_expr(struct Context *ctx);
int exec_strexpr(struct Context *ctx, int* len);
void handle_error(struct Context *ctx);
	
void exec_cmd_dim(struct Context *ctx);
void exec_cmd_dir(struct Context *ctx);
void exec_cmd_end(struct Context *ctx);
void exec_cmd_for(struct Context *ctx);
void exec_cmd_gosub(struct Context *ctx);
void exec_cmd_goto(struct Context *ctx);
void exec_cmd_if(struct Context *ctx);
void exec_cmd_input(struct Context *ctx);
void exec_cmd_let(struct Context*);
void exec_cmd_list(struct Context *ctx);
void exec_cmd_load(struct Context *ctx);
void exec_cmd_new(struct Context *ctx);
void exec_cmd_next(struct Context *ctx);
void exec_cmd_print(struct Context *ctx);
void exec_cmd_rem(struct Context *ctx);
void exec_cmd_return(struct Context *ctx);
void exec_cmd_run(struct Context *ctx);
void exec_cmd_save(struct Context *ctx);
void exec_cmd_then(struct Context *ctx);

void var_clear_all(struct Context *ctx);
void var_add_update_int(struct Context *ctx, const unsigned char *key, int value);
void var_add_update_float(struct Context *ctx, const unsigned char *key, float value);
void var_add_update_string(struct Context *ctx, const unsigned char *key, unsigned char* value, int length);
	
bool compare(const unsigned char *a, const unsigned char *b);
void clear(unsigned char *dst, int size);
void join(unsigned char *dst, const unsigned char *src);
void get_input(unsigned char *s);
bool ensure_token(unsigned char c, int tokenCount, ...);

	// sbparse
#define ISDIGIT(c) (((c)>='0')&&((c)<='9'))
#define ISALPHA(c) (((c)>='A')&&((c)<='Z'))
#define ISalpha(c) (((c)>='a')&&((c)<='z'))
#define ISSPACE(c) (((c)==' ')||((c)=='\t'))
#define ISOP(c) (((c)=='+')||((c)=='-')||((c)=='*')\
                        ||((c)=='/')||((c)=='>')||((c)=='<')\
                        ||((c)=='=')||((c)=='(')||((c)==')')\
                        ||((c)=='%'))

int get_symbol(const unsigned char *s, int i, unsigned char *t);
int get_token(const unsigned char *s, int i, TokenType* token);
int get_int(const unsigned char *s, int i, int *num);
int get_float(const unsigned char* s, int i, float *fv);
int ignore_space(const unsigned char *s, int i);
void to_uppercase(unsigned char *s);
bool isemptyline(unsigned char *linebuf);
int length(const unsigned char *s);
int next_statement(struct Context *ctx);
void tokenize(struct Context* ctx, const unsigned char* input, unsigned char *output);

// Commands are in the range 128–162 / $80–A2
// Various "bywords" that form part of the syntax of the keywords above fall in the 163–169 / $A3–$A9 range
// Arithmetic and logic operators have token codes 170–179 / $AA–$B3
// Functions are in the range 180–202 / $B4–CA

#define TOKEN_END			128	
#define TOKEN_FOR			129	
#define TOKEN_NEXT			130	
#define TOKEN_DATA			131	
#define TOKEN_INPUT_DEV		132	
#define TOKEN_INPUT			133	
#define TOKEN_DIM			134	
#define TOKEN_READ			135	
#define TOKEN_LET			136	
#define TOKEN_GOTO			137	
#define TOKEN_RUN			138	
#define TOKEN_IF			139	
#define TOKEN_RESTORE		140	
#define TOKEN_GOSUB			141	
#define TOKEN_RETURN		142	
#define TOKEN_REM			143	
#define TOKEN_STOP			144	
#define TOKEN_ON			145	
#define TOKEN_WAIT			146	
#define TOKEN_LOAD			147	
#define TOKEN_SAVE			148	
#define TOKEN_VERIFY		149	
#define TOKEN_DEF			150	
#define TOKEN_POKE			151	
#define TOKEN_PRINT_DEV		152	
#define TOKEN_PRINT			153	
#define TOKEN_CONT			154	
#define TOKEN_LIST			155	
#define TOKEN_CLR			156	
#define TOKEN_CMD			157	
#define TOKEN_SYS			158	
#define TOKEN_OPEN			159	
#define TOKEN_CLOSE			160	
#define TOKEN_GET			161	
#define TOKEN_NEW			162	
#define TOKEN_TAB_LP		163	
#define TOKEN_TO			164	
#define TOKEN_FN			165	
#define TOKEN_SPC_LP		166	
#define TOKEN_THEN			167	
#define TOKEN_NOT			168	
#define TOKEN_STEP			169	
#define TOKEN_ADD			170	
#define TOKEN_SUB			171	
#define TOKEN_MULT			172	
#define TOKEN_DIV			173	
#define TOKEN_POW			174	
#define TOKEN_AND			175	
#define TOKEN_OR			176	
#define TOKEN_GT			177	
#define TOKEN_EQUALS		178	
#define TOKEN_LT			179	
#define TOKEN_SGN			180	
#define TOKEN_INT			181	
#define TOKEN_ABS			182	
#define TOKEN_USR			183	
#define TOKEN_FRE			184	
#define TOKEN_POS			185	
#define TOKEN_SQR			186	
#define TOKEN_RND			187	
#define TOKEN_LOG			188		
#define TOKEN_EXP			189	
#define TOKEN_COS			190	
#define TOKEN_SIN			191	
#define TOKEN_TAN			192	
#define TOKEN_ATN			193	
#define TOKEN_PEEK			194	
#define TOKEN_LEN			195	
#define TOKEN_STR$			196	
#define TOKEN_VAL			197	
#define TOKEN_ASC			198	
#define TOKEN_CHR$			199	
#define TOKEN_LEFT$			200	
#define TOKEN_RIGHT$		201	
#define TOKEN_MID$			202	
#define TOKEN_GO			203	
#define TOKEN_DIR			204
}
#endif
/*
  C BASIC

  NOTE: keywords without assignments are OPTIONAL
*/

#define FUNCTION
#define BEGIN_FUNCTION {
#define END_FUNCTION }
#define SUB void
#define LOCAL static
#define BEGIN_SUB {
#define EXIT_SUB return;
#define END_SUB }
#define RETURN_FUNCTION return
#define CALL
#define AND &&
#define OR ||
#define NOT !
#define MOD %
#define ABS abs
#define DIM
#define AS
#define LET
#define INCR ++
#define DECR --
#define IF if
#define BEGIN_IF {
#define THEN {
#define THEN_DO
#define ELSE } else {
#define ELSE_IF } else if
#define END_IF }
#define DEF_FOR for
#define TO ;
#define STEP ;
#define BEGIN_FOR {
#define NEXT }
#define SELECT_CASE switch
#define BEGIN_SELECT {
#define CASE case
#define _TO_ ...
#define END_CASE break;
#define CASE_ELSE default:
#define END_SELECT }
#define DO do {
#define WHILE } while
#define DEF_WHILE while
#define BEGIN_WHILE {
#define WEND }
#define PRINT printf
#define FORMAT sprintf
#define PRINT_FILE fprintf
#define PTR *
#define AT &
#define EQ ==
#define NE !=
#define MAIN int main (int argc, char** argv)
#define END_TYPE }
#define GOTO goto
#define OPEN fopen
#define SEEK fseek
#define REWIND rewind
#define READ fread
#define CLOSE fclose
#define POS ftell
#define LTRIM(x) ltrim(x)
#define RTRIM(x) rtrim(x)
#define TRIM(x) trim(x)


/* Remove leading whitespaces */
char *ltrim(char *const s)
{
        size_t len;
        char *cur;

        if(s && *s) {
                len = strlen(s);
                cur = s;

                while(*cur && isspace(*cur))
                        ++cur, --len;

                if(s != cur)
                        memmove(s, cur, len + 1);
        }

        return s;
}

/* Remove trailing whitespaces */
char *rtrim(char *const s)
{
        size_t len;
        char *cur;

        if(s && *s) {
                len = strlen(s);
                cur = s + len - 1;

                while(cur != s && isspace(*cur))
                        --cur, --len;

                cur[isspace(*cur) ? 0 : 1] = '\0';
        }

        return s;
}

/* Remove leading and trailing whitespaces */
char *trim(char *const s)
{
        rtrim(s);
        ltrim(s);

        return s;
}
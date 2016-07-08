#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef char str_t[120];
str_t st[25];
int sti;
char cur;

bool is_special(char c)
{
	return c == '{' || c == '}' || c == '+' || c == '"' || c == ':';
}

bool is_space(char c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n' ;
}

void assure_no_special(void)
{
	if (is_special(cur)) {
		fprintf(stderr, "err: unexpected special symbol\n");
		exit(1);
	}
}

void assure_no_eof(void)
{
	if (cur == EOF) {
		fprintf(stderr, "err: unexpected eof\n");
		exit(2);
	}
}

bool get_next_char_if(bool expr)
{
	if (!expr)
		return false;

	cur = getchar();
	return true;
}

bool get_next_token_if(bool expr)
{
	if (!get_next_char_if(expr))
		return false;

	while (get_next_char_if(is_space(cur)))
		continue;
	return true;
}

void read_key(char *k)
{
	char *guard = k + sizeof(str_t) - 1;

	assure_no_special();
	do {
		assure_no_eof();
		*k++ = cur;
		if (k == guard) {
			fprintf(stderr, "err: too long key\n");
			exit(3);
		}
		cur = getchar();
	} while (!get_next_token_if(is_space(cur)) && !is_special(cur));
	*k = 0;
}

void tr_value(const char *k);

void tr_paar(void)
{
	char *k = st[sti++];

	if (sti > sizeof(st) / sizeof(*st)) {
		fprintf(stderr, "err: too deep structure\n");
		exit(4);
	}

	read_key(k);

	tr_value(k);
	while (get_next_token_if(cur == '+'))
		tr_value(k);

	--sti;
}

void tr_quoted_string(void)
{
	while (!get_next_token_if(cur == '"')) {
		assure_no_eof();
		printf("%c", cur);
		cur = getchar();
	}
}

void tr_simple_string(void)
{
	assure_no_special();
	assure_no_eof();
	do {
		printf("%c", cur);
		cur = getchar();
	} while (!get_next_token_if(is_space(cur)) && !is_special(cur) && cur != EOF);
}

void tr_value(const char *k)
{
	assure_no_eof();
	printf("<%s>", k);
	if (get_next_token_if(cur == '{')) {
		while (!get_next_token_if(cur == '}'))
			tr_paar();
	} else if (get_next_char_if(cur == '"')) {
		tr_quoted_string();
	} else {
		tr_simple_string();
	}
	printf("</%s>\n", k);
}

int main(int argn, char *argv[])
{
	printf("<?xml version=\"1.0\"?>\n");
	get_next_token_if(true);

	tr_paar();
	while (cur != EOF)
		tr_paar();

	return 0;
}

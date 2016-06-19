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

bool get_next_token_if(bool expr)
{
	if (!expr)
		return false;

	cur = getchar();
	while (is_space(cur))
		cur = getchar();

	return true;
}

void tr_string(void)
{
	if (cur == '"') {
		for (;;) {
			cur = getchar();
			if (get_next_token_if(cur == '"'))
				return;

			assure_no_eof();
			printf("%c", cur);
		}
	}

	assure_no_special();
	assure_no_eof();
	do {
		printf("%c", cur);
		cur = getchar();
	} while (!get_next_token_if(is_space(cur)) && !is_special(cur) && cur != EOF);
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

void tr_value(const char *k)
{
	assure_no_eof();
	printf("<%s>", k);
	if (!get_next_token_if(cur == '{'))
		tr_string();
	else
		while (!get_next_token_if(cur == '}'))
			tr_paar();
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

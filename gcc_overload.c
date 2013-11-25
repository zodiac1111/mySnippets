#include <stdio.h>
struct s1
{
	int a;
	int b;

	double c;
};

struct s2
{
	long long a;
	long long b;
};

void gcc_overload_s1(struct s1 s)
{
	printf("in %s\n", __FUNCTION__);
}

void gcc_overload_s2(struct s2 s)
{
	printf("in %s\n", __FUNCTION__);
}
void gcc_overload_int(int s)
{
	printf("in %s\n", __FUNCTION__);
}

/* warning: dereferencing type-punned pointer will break strict-aliasing rules */
#define gcc_overload(A)\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(A), struct s1),\
		gcc_overload_s1(*(struct s1 *)&A),\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(A), struct s2),\
		gcc_overload_s2(*(struct s2 *)&A),\
	__builtin_choose_expr(__builtin_types_compatible_p(typeof(A), int),\
		gcc_overload_int(*(int *)&A),\
		(void)0)))

int main(void)
{
	struct s1 a ;
	struct s2 b ;
	int c;
	gcc_overload(a);
	gcc_overload(b);
	gcc_overload(c);
	return 0;
}

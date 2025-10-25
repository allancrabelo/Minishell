#include "minishell.h"

static void	skip_whitespace_and_sign(const char **str, int *sign)
{
	while (**str == ' ' || (**str >= 9 && **str <= 13))
		(*str)++;
	if (**str == '+' || **str == '-')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}

static int	parse_digits(const char **str, unsigned long long *acum,
				unsigned long long lim)
{
	unsigned int	digit;

	while (**str && ft_isdigit((unsigned char)**str))
	{
		digit = (unsigned)(**str - '0');
		if (*acum > (lim - digit) / 10ULL)
			return (0);
		*acum = *acum * 10ULL + digit;
		(*str)++;
	}
	if (**str != '\0')
		return (0);
	return (1);
}

int	ft_atoll_safe(const char *str, long long *result)
{
	int					sign;
	unsigned long long	acum;
	unsigned long long	lim;

	sign = 1;
	acum = 0;
	lim = (unsigned long long)LLONG_MAX;
	skip_whitespace_and_sign(&str, &sign);
	if (!*str)
		return (0);
	if (sign < 0)
		lim = (unsigned long long)LLONG_MAX + 1ULL;
	if (!parse_digits(&str, &acum, lim))
		return (0);
	if (sign < 0)
		*result = (long long)(-acum);
	else
		*result = (long long)acum;
	return (1);
}

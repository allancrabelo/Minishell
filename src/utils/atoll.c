#include "minishell.h"

/**
 * @brief Skips whitespace and processes optional sign character
 *
 * Advances string pointer past any leading whitespace and
 * handles optional '+' or '-' sign, updating the sign flag.
 *
 * @param str Double pointer to current position in input string
 * @param sign Pointer to sign flag (1 for positive, -1 for negative)
 * @return void
 */
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

/**
 * @brief Parses digit sequence with overflow protection
 *
 * Processes numeric digits from string, accumulating value
 * while checking for arithmetic overflow at each step.
 *
 * @param str Double pointer to current position in input string
 * @param acum Pointer to accumulator for numeric value
 * @param lim Maximum allowed value for overflow checking
 * @return int 1 if valid digits parsed, 0 on overflow or invalid characters
 */

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

/**
 * @brief Safe string to long long conversion with overflow checking
 *
 * Converts string to long long with comprehensive error checking:
 * - Handles whitespace and optional sign
 * - Detects overflow/underflow
 * - Validates digit sequence
 *
 * @param str String to convert
 * @param result Pointer to store conversion result
 * @return int 1 on success, 0 on conversion error
 */
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

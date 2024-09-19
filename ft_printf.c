/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmravec <mmravec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:27:51 by mmravec           #+#    #+#             */
/*   Updated: 2024/09/19 18:51:03 by mmravec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_char(int c)
{
	return (write(1, &c, 1));
}

int	print_string(char *s)
{
	int		count;

	count = 0;
	if (s == NULL)
		return (write(1, "(null)", 6));
	while (*s)
	{
		if (*s >= 32 && *s <= 126)
			count += write(1, s, 1);
		s++;
	}
	return (count);
}

int	print_digit(long n, int base, char *symbols)
{
	int		count;

	count = 0;
	if (n < 0)
	{
		write(1, "-", 1);
		return (print_digit( -n, base, symbols) + 1);
	}
	else if (n < base)
		return (print_char(symbols[n]));
	else
	{
		count = print_digit(n / base, base, symbols);
		return (count + print_digit(n % base, base, symbols));
	}
}

int	print_unsigned_digit(unsigned long long n, int base, char *symbols)
{
	int		count;
	char	buffer[65];
	int		i;

	count = 0;
	i = 0;
	if (n == 0)
		return (print_char('0'));
	while (n > 0)
	{
		buffer[i++] = symbols[n % base];
		n /= base;
	}
	while (--i >= 0)
		count += print_char(buffer[i]);

	return (count);
}


int	print_pointer(void *p)
{
	int		count;
	uintptr_t address;

	address = (uintptr_t)p;
	count = 0;
	count += write(1, "0x", 2);
	count += print_unsigned_digit(address, 16, "0123456789abcdef");
	return (count);
}

int	print_format(char conversion, va_list *ap)
{
	int		count;
	char	*base;
	char	*base_small;

	base_small = "0123456789abcdef";
	base = "0123456789ABCDEF";

	count = 0;
	if (conversion == 'c')
		count += print_char(va_arg(*ap, int));
	else if (conversion == 's')
		count += print_string(va_arg(*ap, char *));
	else if (conversion == 'd' || conversion == 'i')
		count += print_digit((long)va_arg(*ap, int), 10, "0123456789");
	else if (conversion == 'u')
		count += print_digit(va_arg(*ap, unsigned int), 10, "0123456789");
	else if (conversion == 'x')
		count += print_digit((long)va_arg(*ap, unsigned int), 16, base_small);
	else if (conversion == 'X')
		count += print_digit((long)va_arg(*ap, unsigned int), 16, base);
	else if (conversion == 'p')
		count += print_pointer(va_arg(*ap, void *));
	else
		count += write(1, &conversion, 1);

	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		count;

	va_start(ap, format);
	count = 0;
	while (*format != '\0')
	{
		if (*format == '%')
			count += print_format(*++format, &ap);
		else
			count += write(1, format, 1);
		++format;
	}
	va_end(ap);
	return (count);
}

#include <limits.h>

int main(void)
{
    int     count;
    int     x = 42;
    int     *ptr = &x;

    count = ft_printf("Hello %s!\n", "John");
    ft_printf("Chars written are: %d\n", count);

    count = ft_printf("%X\n", 42);
    ft_printf("Chars written are: %d\n", count);

    count = ft_printf("%u\n", -10);  // Be careful with unsigned numbers
    ft_printf("Chars written are: %d\n", count);

    // Testing %p conversion
    count = ft_printf("Pointer to x: %p\n", (void*)ptr);
    ft_printf("Chars written are: %d\n", count);

    // Testing NULL pointer
    count = ft_printf("NULL pointer: %p\n", NULL);
    ft_printf("Chars written are: %d\n", count);

	count = ft_printf(" %c \n", '1');
	ft_printf("Chars written are: %d\n", count);

	count = ft_printf(" %c %c %c \n", '1', 0, '3');
	ft_printf("Chars written are: %d\n", count);

	ft_printf(" %p %p \n", (void*)(uintptr_t)LONG_MIN, (void*)(uintptr_t)LONG_MAX);  // Safe cast to uintptr_t
	ft_printf(" %p %p \n", (void*)(uintptr_t)ULONG_MAX, (void*)(uintptr_t)(-ULONG_MAX));  // Safe cast to uintptr_t

    return 0;
}

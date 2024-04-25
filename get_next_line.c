/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 18:38:10 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/04/25 19:06:22 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

static size_t	gnl_strlen(char *s)
{
	size_t	len;

	len = 0;
	if (s)
		while (s[len])
			len++;
	return (len);
}

static void	gnl_strcat(char *dest, char *src)
{
	size_t	i;
	size_t	j;
	size_t	dest_len;
	size_t	total_len;

	dest_len = gnl_strlen(dest);
	total_len = dest_len + gnl_strlen(src);
	i = dest_len;
	j = 0;
	while (i < total_len && src[j] != '\0')
	{
		dest[i] = src[j];
		j++;
		i++;
	}
	dest[i] = '\0';
}

static char	*gnl_strjoin(char *s1, char *s2)
{
	char	*join;
	size_t	join_len;
	char	*join_tmp;

	join_len = gnl_strlen(s1) + gnl_strlen(s2) + 1;
	join = (char *) malloc(join_len * sizeof(char));
	if (join == NULL)
		return (NULL);
	join_tmp = join;
	if (s1)
		while (*s1)
			*join_tmp++ = *s1++;
	*join_tmp = '\0';
	gnl_strcat(join, s2);
	return (join);
}

static char	*gnl_strchr(char *s, int c)
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char) c)
			return (&s[i]);
		i++;
	}
	if (s[i] == (char) c)
		return (&s[i]);
	return (0);
}

static void	*gnl_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_tmp;
	const unsigned char	*src_tmp;

	if (!dest && !src)
		return (NULL);
	dest_tmp = dest;
	src_tmp = src;
	if (dest_tmp < src_tmp)
	{
		while (n--)
			*dest_tmp++ = *src_tmp++;
	}
	else
	{
		dest_tmp += n;
		src_tmp += n;
		while (n--)
			*--dest_tmp = *--src_tmp;
	}
	return (dest);
}

static char	*append_buffer(char *buffer, char *line, char *sep, char *nl_ptr)
{
	char	*temp_line;
	char	*old_line;

	if (nl_ptr)
		*nl_ptr = '\0';
	temp_line = gnl_strjoin(buffer, sep);
	if (!temp_line)
		return (NULL);
	old_line = line;
	line = gnl_strjoin(line, temp_line);
	if (!line)
		return (NULL);
	free(old_line);
	free(temp_line);
	if (nl_ptr)
		gnl_memmove(buffer, nl_ptr + 1, gnl_strlen(nl_ptr + 1) + 1);
	else
		buffer[0] = '\0';
	return (line);
}

static int	read_buffer(int fd, char *buffer, ssize_t *read_ret)
{
	*read_ret = read(fd, buffer, BUFFER_SIZE);
	if (*read_ret <= 0)
		return (0);
	buffer[*read_ret] = '\0';
	return (1);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	char		*nl_ptr;
	ssize_t		read_ret;

	line = NULL;
	if (fd < 0)
		return (NULL);
	while (1)
	{
		if (!buffer[0])
			if (!read_buffer(fd, buffer, &read_ret))
				break ;
		nl_ptr = gnl_strchr(buffer, '\n');
		if (nl_ptr)
			return (line = append_buffer(buffer, line, "\n", nl_ptr));
		else
			line = append_buffer(buffer, line, "", NULL);
	}
	if (read_ret == -1)
		return (NULL);
	return (line);
}

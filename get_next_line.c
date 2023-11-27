/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 16:46:44 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/27 20:06:26 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	remain[FD_SIZE][BUFFER_SIZE];
	int			b_read;
	t_buff_ctrl	buff_ctrl;
	t_nl		nl;

	if (fd < 0 || fd > FD_SIZE || BUFFER_SIZE <= 0)
		return (NULL);
	if (is_line(remain[fd], &nl, BUFFER_SIZE))
		return (extract_remaining_line(remain[fd], nl.index));
	init_t_buff_ctrl(&buff_ctrl);
	if (!buff_ctrl.first_node)
		return (NULL);
	buff_ctrl.node = buff_ctrl.first_node;
	b_read = read(fd, buff_ctrl.node->buff, BUFFER_SIZE);
	if (check_failure(buff_ctrl, b_read, gnl_strlen(remain[fd])))
		return (NULL);
	while (!is_line(buff_ctrl.node->buff, &nl, b_read) && b_read == BUFFER_SIZE)
	{
		if (check_failure(add_back(&buff_ctrl), b_read, gnl_strlen(remain[fd])))
			return (NULL);
		b_read = read(fd, buff_ctrl.node->buff, BUFFER_SIZE);
	}
	return (build_line(buff_ctrl, remain[fd], nl, b_read));
}

char	*build_line(t_buff_ctrl buff_ctrl, char *remain, t_nl nl, int b_read)
{
	t_build_l	bl;

	bl.i = 0;
	bl.b_remain = b_read - (nl.index + 1);
	buff_ctrl.node = buff_ctrl.first_node;
	if (nl.is_found)
		b_read = nl.index + 1;
	bl.line = malloc(sizeof(char) * ((buff_ctrl.node_counter - 1) * BUFFER_SIZE
				+ gnl_strlen(remain) + b_read + 1));
	if (!bl.line)
		return (bl.line);
	bl.i = gnl_strlcpy(bl.line, remain, gnl_strlen(remain));
	while (buff_ctrl.node_counter-- > 1)
	{
		bl.i += gnl_strlcpy(bl.line + bl.i, buff_ctrl.node->buff, BUFFER_SIZE);
		buff_ctrl.node = buff_ctrl.node->next;
	}
	bl.i += gnl_strlcpy(bl.line + bl.i, buff_ctrl.node->buff, b_read);
	if (nl.is_found)
		gnl_strlcpy(remain, buff_ctrl.node->buff + nl.index + 1, bl.b_remain);
	else
		remain[0] = '\0';
	remain[bl.b_remain] = '\0';
	bl.line[bl.i] = '\0';
	return (free_t_buffer_chunk(buff_ctrl.first_node), bl.line);
}

unsigned int	check_failure(t_buff_ctrl buff_ctrl, int b_read,
		size_t len_remain)
{
	if (b_read <= 0 && !len_remain)
	{
		free_t_buffer_chunk(buff_ctrl.first_node);
		return (1);
	}
	if (!buff_ctrl.node)
	{
		free_t_buffer_chunk(buff_ctrl.first_node);
		return (1);
	}
	return (0);
}

char	*extract_remaining_line(char *remain, size_t nl_index)
{
	char	*extracted_line;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	extracted_line = malloc(sizeof(char) * (nl_index + 2));
	if (!extracted_line)
		return (extracted_line);
	while (i != nl_index + 1)
	{
		extracted_line[i] = remain[i];
		i++;
	}
	extracted_line[i] = '\0';
	while (remain[i])
		remain[j++] = remain[i++];
	remain[j] = remain[i];
	return (extracted_line);
}

unsigned int	is_line(char *buffer, t_nl *nl, size_t b_read)
{
	size_t	i;

	i = 0;
	nl->is_found = 0;
	nl->index = 0;
	if (!(*buffer))
		return (nl->is_found);
	while (i < b_read && buffer[i])
	{
		if (buffer[i] == '\n')
		{
			nl->is_found = 1;
			nl->index = i;
			break ;
		}
		i++;
	}
	return (nl->is_found);
}

// #include <stdio.h>
// #include <fcntl.h>
// int main()
//  {
//     int fd = open("alternate", O_RDWR);
//     // printf("%i\n", fd);
//     // char *line = get_next_line(fd);
//     // printf("%s\n", line);
//     // free (line);
//     // printf("%i\n", FD_SIZE);
//     int i ;
//     char * line;
//     i = 0;
//     while (i < 10)
//     {
//         line = get_next_line(0);
//         printf("%s", line);
//         free (line);
//         i++;
//     }
//     return (1);
// }

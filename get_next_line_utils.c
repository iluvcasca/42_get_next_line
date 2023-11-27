/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 20:06:44 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/27 20:06:48 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	i;

	src_len = dstsize;
	i = 0;
	if (!dstsize)
		return (src_len);
	while (dstsize && src[i])
	{
		dst[i] = src[i];
		dstsize--;
		i++;
	}
	return (src_len);
}

void	free_t_buffer_chunk(t_buffer_chunk *first)
{
	t_buffer_chunk	*current;
	t_buffer_chunk	*next;

	current = first;
	while (current->next)
	{
		next = current->next;
		free(current);
		current = next;
	}
	free(current);
}

t_buff_ctrl	add_back(t_buff_ctrl *buff_ctrl)
{
	t_buffer_chunk	*last;

	last = malloc(sizeof(*last));
	buff_ctrl->node->next = last;
	buff_ctrl->node = last;
	buff_ctrl->node_counter++;
	if (!last)
		return (*buff_ctrl);
	last->next = NULL;
	last->buff[0] = 0;
	return (*buff_ctrl);
}

void	init_t_buff_ctrl(t_buff_ctrl *buff_ctrl)
{
	t_buffer_chunk	*first_node;

	first_node = malloc(sizeof(*first_node));
	if (!first_node)
		return ;
	first_node->buff[0] = 0;
	first_node->next = NULL;
	buff_ctrl->first_node = first_node;
	buff_ctrl->node_counter = 1;
}

size_t	gnl_strlen(char *buffer)
{
	size_t	i;

	i = 0;
	while (buffer[i])
		i++;
	return (i);
}

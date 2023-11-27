/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 16:46:44 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/27 14:00:52 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *get_next_line(int fd)
{
    static char remain_of_buffer[FD_SETSIZE][BUFFER_SIZE];
    int bytes_read;
    t_buffer_chunk_control buffer_chunk_control;
    t_newline newline;
    
    if (fd < 0 || fd > FD_SETSIZE || !BUFFER_SIZE)
        return (NULL);
    if (is_line(remain_of_buffer[fd], &newline, BUFFER_SIZE))      
        return (extract_remaining_line(remain_of_buffer[fd], newline.index));
    init_t_buffer_control(&buffer_chunk_control);
    if (!buffer_chunk_control.first_node)
        return (NULL);
    buffer_chunk_control.node = buffer_chunk_control.first_node;
    bytes_read = read(fd, buffer_chunk_control.node->buffer, BUFFER_SIZE);
    if (check_failure(buffer_chunk_control, bytes_read, gnl_strlen(remain_of_buffer[fd])))
        return (NULL);
    while (!is_line(buffer_chunk_control.node->buffer, &newline, bytes_read) && bytes_read == BUFFER_SIZE)
    {
        if (check_failure(add_back_t_buffer_chunk(&buffer_chunk_control), bytes_read, gnl_strlen(remain_of_buffer[fd])))
            return (NULL);
        bytes_read = read(fd, buffer_chunk_control.node->buffer, BUFFER_SIZE);
    }
    return (build_line(buffer_chunk_control, remain_of_buffer[fd], newline, bytes_read));
}

char *build_line(t_buffer_chunk_control buffer_chunk_control, char * remain_buffer, t_newline newline, int bytes_read)
{
    size_t i;
    char * line;
    size_t s1 = ((buffer_chunk_control.node_counter - 1) * BUFFER_SIZE + newline.index + 2 + gnl_strlen(remain_buffer));
    size_t s2 = ((buffer_chunk_control.node_counter - 1) * BUFFER_SIZE + bytes_read + 1 + gnl_strlen(remain_buffer));
    i = 0;
    buffer_chunk_control.node = buffer_chunk_control.first_node;
    if (newline.is_found)
        line = malloc(sizeof(char) * s1);
    else
        line = malloc(sizeof(char) * s2);
    if (!line)
        return (line);
    i = gnl_strlcpy(line, remain_buffer, gnl_strlen(remain_buffer)); 
    while (buffer_chunk_control.node_counter > 1)
    {
        i += gnl_strlcpy(line + i, buffer_chunk_control.node->buffer, BUFFER_SIZE);
        buffer_chunk_control.node = buffer_chunk_control.node->next;
        buffer_chunk_control.node_counter--;
    }
    if (newline.is_found)
    {
        i += gnl_strlcpy(line + i, buffer_chunk_control.node->buffer, newline.index + 1);
        gnl_strlcpy(remain_buffer, buffer_chunk_control.node->buffer + newline.index + 1, bytes_read - (newline.index + 1));
        remain_buffer[bytes_read - (newline.index + 1)] = '\0';
    }
    else
{
        i += gnl_strlcpy(line + i, buffer_chunk_control.node->buffer, bytes_read);
        remain_buffer[0] = '\0';
    }
    line[i] = '\0';
    free_t_buffer_chunk(buffer_chunk_control.first_node);
    return (line);
}

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

unsigned int check_failure(t_buffer_chunk_control buffer_chunk_control, int bytes_read, size_t len_remain)
{
    if (bytes_read <= 0 && !len_remain)
    {
        free_t_buffer_chunk (buffer_chunk_control.first_node);
        return (1);
    }
    if (!buffer_chunk_control.node)
    {
        free_t_buffer_chunk (buffer_chunk_control.first_node);
        return (1);
    }
    return (0);
}

char *extract_remaining_line(char * remain_buffer, size_t newline_index)
{
    char * extracted_line;
    size_t i;
    size_t j;

    i = 0;
    j = 0;
    extracted_line = malloc(sizeof(char)*(newline_index + 2));
    if (!extracted_line)
        return (extracted_line);
    while (i != newline_index + 1)
    {
        extracted_line[i] = remain_buffer[i];
        i++;
    }
    extracted_line[i] = '\0';
    while (remain_buffer[i])
        remain_buffer[j++] = remain_buffer[i++];
    remain_buffer[j] = remain_buffer[i];
    return (extracted_line);
}

unsigned int  is_line (char * buffer, t_newline * newline, size_t bytes_read)
{
    size_t i;
    i = 0;
    newline->is_found = 0;
    newline->index = 0;
    if (!(*buffer))
        return (newline->is_found);
    while (i < bytes_read && buffer[i])
    {
        if (buffer[i] == '\n')
        {
            newline->is_found = 1;
            newline->index = i;
            break;
        }
        i++;
    }
    return (newline->is_found);
}

void free_t_buffer_chunk(t_buffer_chunk *first)
{
    t_buffer_chunk * current;
    t_buffer_chunk * next;
    current = first;
    while (current->next)
    {
        next = current->next;
        free (current);
        current = next;
    }
    free (current);
}

t_buffer_chunk_control add_back_t_buffer_chunk (t_buffer_chunk_control * buffer_chunk_control)
{
    t_buffer_chunk *last;
    last = malloc(sizeof(*last));
    buffer_chunk_control->node->next = last;
    buffer_chunk_control->node = last;
    buffer_chunk_control->node_counter++;
    if (!last)
        return (*buffer_chunk_control);
    last->next = NULL;
    last->buffer[0] = 0;
    return (*buffer_chunk_control);
}

void init_t_buffer_control (t_buffer_chunk_control * buffer_chunk_control)
{
    t_buffer_chunk *first_node;
    first_node = malloc(sizeof(*first_node));
    if (!first_node)
        return ;
    first_node->buffer[0] = 0;
    first_node->next = NULL;
    buffer_chunk_control->first_node=first_node;
    buffer_chunk_control->node_counter = 1;
}

size_t gnl_strlen(char * buffer)
{
    size_t i;

    i = 0;
    while (buffer[i])
        i++;
    return (i);
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
//     // printf("%i\n", FD_SETSIZE);
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

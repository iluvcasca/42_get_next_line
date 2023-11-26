/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:31:19 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/26 16:35:35 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *get_next_line(int fd)
{
    static char remain_buffer[FD_SETSIZE][BUFFER_SIZE];
    char * line;
    t_newline  newline;
    t_buffer_chunk_control buffer_chunk_control;
    t_buffer_chunk * node;
    size_t bytes_read;

    if (fd < 0 || fd > FD_SETSIZE)
        return (NULL);
    is_line(remain_buffer[fd], &newline, BUFFER_SIZE);
    if (newline.is_found)
        return (extract_remaining_line(remain_buffer[fd], newline.index));
    buffer_chunk_control.first_node = add_back_t_buffer_chunk(NULL);
    if (!buffer_chunk_control.first_node)
        return (NULL);
    buffer_chunk_control.node_counter = 1;
    node = buffer_chunk_control.first_node;
    bytes_read = read(fd, node->buffer, BUFFER_SIZE);
    if (bytes_read <= 0)
    {
        free_t_buffer_chunk (buffer_chunk_control.first_node);
        return (NULL);
    }
    is_line(node->buffer, &newline, bytes_read);
    while (!newline.is_found || bytes_read < BUFFER_SIZE) 
    {
        if (!add_back_t_buffer_chunk(node))
        {
            free_t_buffer_chunk(buffer_chunk_control.first_node);
            return (NULL);
        }
        buffer_chunk_control.node_counter++;
        node = node->next;
        bytes_read = read(fd, node->buffer, BUFFER_SIZE);
        is_line(node->buffer, &newline, bytes_read);
    }
        line = build_line(buffer_chunk_control.first_node, remain_buffer[fd], buffer_chunk_control.node_counter, newline.index, bytes_read); 
        free_t_buffer_chunk (buffer_chunk_control.first_node);
    return (line);
}

void  is_line (char * buffer, t_newline * newline, size_t bytes_read)
{
    size_t i;
    i = 0;
    newline->is_found = 0;
    newline->index = 0;
    if (!(*buffer))
        return ;
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
}

t_buffer_chunk *add_back_t_buffer_chunk (t_buffer_chunk *current)
{
    t_buffer_chunk *last;
    last = malloc(sizeof(*last));
    if (!last)
        return (NULL);
    last->next = NULL;
    last->buffer[0] = 0;
    if (!current)
        return (last);
    current->next = last;
    return (last);
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

char *build_line(t_buffer_chunk * first, char * remain_buffer, unsigned int node_counter, size_t newline_index, size_t bytes_read)
{
    char * line;
    size_t i;
    size_t j;
    size_t k;

    i = -1;
    k = 0;
    if (newline_index == 0)
        newline_index = BUFFER_SIZE - 1;
    line = malloc(sizeof(char)*((bytes_read*(node_counter - 1) + newline_index + 2) + gnl_strlen(remain_buffer)));
    if (!line)
        return (line);
    while (remain_buffer[++i])
        line [i] = remain_buffer[i];
    while (node_counter--) 
    {
        j = 0;
        while ((first->buffer)[j] != '\n' && j < bytes_read - 1)
            line [i++] = (first->buffer)[j++];  
        line [i++] = (first->buffer)[j++];
        while (j<bytes_read)
            remain_buffer[k++] = (first->buffer)[j++];
        remain_buffer[k] = '\0';
        first = first->next;
    }
    line[i] = '\0';
    return (line);
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
//     int fd = open("Quran.txt", O_RDONLY);
//     // printf("%i\n", fd);
//     // char *line = get_next_line(fd);
//     // printf("%s\n", line);
//     // free (line);
//     // printf("%i\n", FD_SETSIZE);
//     int i ;
//     char * line;
//     i = 0;
//     while (i < 14998)
//     {
//         line = get_next_line(-5);
//         printf("%s", line);
//         free (line);
//         i++;
//     }
//     return (1);
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:31:19 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/25 16:18:39 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *get_next_line(int fd)
{
    static char remain_buffer[FD_SETSIZE][BUFFER_SIZE];
    char * line;
    size_t * is_found_and_index;
    t_buffer_chunk * first_node;
    t_buffer_chunk * node;
    unsigned int node_counter;
    is_found_and_index = (size_t[2]){0};
    is_found_and_index = is_line(remain_buffer[fd], is_found_and_index);
    if (is_found_and_index[0])
        return (extract_remaining(remain_buffer[fd], is_found_and_index[1]));
    first_node = add_back_t_buffer_chunk(NULL);
    node_counter = 1;
    node = first_node;
    if (read(fd, node->buffer, BUFFER_SIZE) <= 0)
    {
        free_t_buffer_chunk (&first_node);
        return (NULL);
    }
    is_found_and_index = is_line(node->buffer, is_found_and_index);
    while (!is_found_and_index[0]) 
    {
        add_back_t_buffer_chunk(node);
        node_counter++;
        node = node->next;
        read(fd, node->buffer, BUFFER_SIZE);
        is_found_and_index = is_line(node->buffer, is_found_and_index);
    }
        line = build_line(first_node, remain_buffer[fd], node_counter, is_found_and_index[1]); 
        free_t_buffer_chunk (&first_node);
    return (line);
}

size_t  * is_line (char * buffer, size_t is_found_and_index[2])
{
    size_t i;
    size_t j;
    i = 0;
    j = 0;
    if (!(*buffer))
        return is_found_and_index;
    while (i < BUFFER_SIZE && buffer[i])
    {
        if (buffer[i] == '\n')
        {
            is_found_and_index[0] = 1;
            is_found_and_index[1] = i;
            break;
        }
        i++;
    }
    return is_found_and_index;
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

void free_t_buffer_chunk(t_buffer_chunk **first)
{
    t_buffer_chunk * current;
    t_buffer_chunk * next;
    current = *first;
    while (current->next)
    {
        next = current->next;
        free (current);
        current = next;
    }
    free (current);
}

char *build_line(t_buffer_chunk * first, char * remain_buffer, unsigned int node_counter, size_t newline_index)
{
    char * line;
    size_t i;
    size_t j;
    size_t k;

    i = 0;
    k = 0;
    line = malloc(sizeof(char)*((BUFFER_SIZE*(node_counter - 1) + newline_index + 2) + gnl_strlen(remain_buffer)));
    while (remain_buffer[i])
    {
        line [i] = remain_buffer[i];
        i++;
    }
    while (node_counter--) 
    {
        j = 0;
        while ((first->buffer)[j] != '\n' && j < BUFFER_SIZE - 1)
        {
            line [i] = (first->buffer)[j];  
            i++;
            j++;
        }
        line [i++] = (first->buffer)[j++];
        while (j<BUFFER_SIZE)
            remain_buffer[k++] = (first->buffer)[j++];
        remain_buffer[k] = '\0';
        first = first->next;
    }
    line[i] = '\0';

    return (line);
}

char *extract_remaining(char * remain_buffer, size_t newline_index)
{
    size_t i;
    size_t j;
    char * extracted_line;

    i = 0;
    j = 0;
    extracted_line = malloc(sizeof(char)*(newline_index + 2));
    while (i != newline_index + 1)
    {
        extracted_line[i] = remain_buffer[i];
        i++;
    }
    extracted_line[i] = '\0';
    while (remain_buffer[i])
    {
        remain_buffer[j] = remain_buffer[i];
        j++;
        i++;
    }
    remain_buffer[j] = remain_buffer[i];
    
    return (extracted_line);
}

size_t gnl_strlen(char * buffer)
{
    size_t i;

    i = 0;
    while (buffer[i] && i < BUFFER_SIZE)
        i++;
    return (i);
}

#include <stdio.h>
#include <fcntl.h>
int main()
 {
    int fd = open("Quran.txt", O_RDONLY);
    // printf("%i\n", fd);
    // char *line = get_next_line(fd);
    // printf("%s\n", line);
    // free (line);
    // printf("%i\n", FD_SETSIZE);
    int i ;
    char * line;
    i = 0;
    while (i < 14998)
    {
        line = get_next_line(fd);
        printf("%s", line);
        free (line);
        i++;
    }
    return (1);
}

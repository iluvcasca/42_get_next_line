/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:31:19 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/21 22:22:36 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *get_next_line(int fd)
{
    t_line_start *line_start;
    t_line_char *current;
    int read_return_value;
    int count;
    char *line;
    
    line_start = malloc(sizeof(*line_start));
    count = 0;
    if (!line_start)
    {
        free_get_next_line(line_start);
        return (NULL);
    }
    line_start->fd = fd;
    line_start->start = add_line_char (line_start, NULL);
    current = line_start->start;
    if (!current)
    {
        free_get_next_line(line_start);
        return NULL;
    }
    if ( fd < 0 )
        return (NULL);
    while ((read_return_value = read(fd, current->character,1)))
    {
        count++;
        if (read_return_value <= 0)
        {
            free_get_next_line(line_start);
            return (NULL);
        }
        if (*(current->character) == '\n')
            break;
        (void)add_line_char(line_start, current);
        current = current->next;
        if (!current)
        {
            free_get_next_line(line_start);
            return (NULL);
        }
    }
    line = malloc(sizeof(char)*count); 
    if (!line)
    {
        free_get_next_line(line_start);
        return (NULL);
    }
    current = line_start->start;
    count = 0;
    while (*(current->character) != '\n')
    {
        line[count] = *(current->character);
        count++;
        current = current->next;
    }
    line[count] = '\0';
    free_get_next_line(line_start);    
    return (line); 
}

void free_get_next_line(t_line_start * line_start)
{
    t_line_char *current;
    t_line_char *next;
    if (line_start->start) 
    {
        current = line_start->start;
        next = current->next;
        while(next)
        {
            free(current);
            current = next;
            next = next->next;
        }
        free (current);
    }
    free (line_start);
}

t_line_char *add_line_char (t_line_start *start, t_line_char *last) {
    t_line_char *new;

    new = malloc (sizeof(*new));

    if (!new)
        return (NULL);
    if (last)
        last->next = new;
    new->next = NULL;    
    return (new);
}

#include <stdio.h>
#include <fcntl.h>
int main()
{
    int fd = open("Quran.txt", O_RDONLY);
    printf("%i\n", fd);
    char *line = get_next_line(fd);
    printf("%s\n", line);
    free (line);
    return (1);
}

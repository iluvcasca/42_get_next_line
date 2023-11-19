/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:31:19 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/19 16:17:55 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *get_next_line(int fd)
{
    char buffer[BUFFER_SIZE];
    char *line;
    static int line_start;
    int line_end;
    int i;
    line_end = line_start;
    i = 0;
    do {
        read(fd, buffer, 1);
        line_end++;
    }
    while (buffer[line_end] != '\n');
    line = malloc (sizeof(char) * (line_end + 1));
    line[line_end] = '\0';
    while (buffer[i] != '\n')
    {
        line[i] = buffer[i];
        i++;
    }
    line_start = line_end + 1;
    return (line);    
}

#include <stdio.h>
#include <fcntl.h>

int main()
{
    int fd = open("test.txt",O_RDONLY);
    if ( fd == -1 )
    {
        printf ("error\n");
        return (0);
    }
    int i = 0;
    char * line;
    while (i != 41)
    {
        line = get_next_line(fd);
        printf("%s\n", line);
        free(line);
        i++;
    }
    return (1);
}

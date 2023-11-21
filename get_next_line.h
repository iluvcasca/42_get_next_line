/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:22:00 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/21 21:34:05 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFFER_SIZE 42
# include <stdlib.h>
# include <unistd.h>

typedef struct s_line_char {
    char *character;
    struct s_line_char *next;
} t_line_char;

typedef struct s_line_start {
    int fd;
    struct s_line_char *start;
} t_line_start;

char *get_next_line(int fd);
void free_get_next_line(t_line_start * line_start);
t_line_char *add_line_char (t_line_start *start, t_line_char *last);
#endif

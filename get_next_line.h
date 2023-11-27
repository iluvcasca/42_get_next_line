/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:22:00 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/27 16:10:23 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif
# include <stdlib.h>
# include <unistd.h>
# define FD_SIZE 500 
typedef struct s_buffer_chunk {
    struct s_buffer_chunk *next;
    char buffer[BUFFER_SIZE];
} t_buffer_chunk;

typedef struct s_buffer_control {
    struct s_buffer_chunk * first_node;
    struct s_buffer_chunk * node;
    unsigned int node_counter;
} t_buffer_control;

typedef struct s_newline {
    unsigned int is_found;
    size_t index;
} t_newline;

char *get_next_line(int fd);
t_buffer_control add_back_t_buffer_chunk (t_buffer_control * buffer_control);
void init_t_buffer_control (t_buffer_control * buffer_control);
void free_t_buffer_chunk(t_buffer_chunk *first);
unsigned int  is_line (char * buffer, t_newline * newline, size_t bytes_read);
char *extract_remaining_line(char * remain_buffer, size_t newline_index);
unsigned int check_failure(t_buffer_control buffer_control, int bytes_read, size_t len_remain);
char *build_line(t_buffer_control buffer_control, char * remain_buffer, t_newline newline, int bytes_read);
size_t gnl_strlen(char * buffer);
size_t	gnl_strlcpy(char *dst, const char *src, size_t dstsize);
#endif

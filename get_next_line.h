/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:22:00 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/26 16:19:46 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif
# include <stdlib.h>
# include <unistd.h>
# include <sys/select.h>

typedef struct s_buffer_chunk {
    struct s_buffer_chunk *next;
    char buffer[BUFFER_SIZE];
} t_buffer_chunk;

typedef struct s_buffer_chunk_control {
    struct s_buffer_chunk * first_node;
    unsigned int node_counter;
} t_buffer_chunk_control;

typedef struct s_newline {
    unsigned int is_found;
    size_t index;
} t_newline;

char *get_next_line(int fd);
void  is_line (char * buffer, t_newline * newline, size_t bytes_read);
char *build_line(t_buffer_chunk * first, char * remain_buffer, unsigned int node_counter, size_t newline_index, size_t bytes_read);
t_buffer_chunk *add_back_t_buffer_chunk (t_buffer_chunk *current);
void free_t_buffer_chunk(t_buffer_chunk *first);
size_t gnl_strlen(char * buffer);
char *extract_remaining_line(char * remain_buffer, size_t newline_index);
#endif

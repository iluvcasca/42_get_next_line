/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:22:00 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/23 20:11:53 by kgriset          ###   ########.fr       */
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
char *get_next_line(int fd);
size_t is_line (char * buffer);
char *build_line(t_buffer_chunk * first, char * remain_buffer, unsigned int node_counter, size_t newline_index);
t_buffer_chunk *add_back_t_buffer_chunk (t_buffer_chunk *current);
void free_t_buffer_chunk(t_buffer_chunk **first);
size_t gnl_strlen(char * buffer);
char *extract_remaining(char * remain_buffer, size_t newline_index);
#endif

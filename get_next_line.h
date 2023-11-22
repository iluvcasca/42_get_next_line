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
typedef struct s_buffer_chunk {
    struct s_buffer_chunk *next;
    char buffer[BUFFER_SIZE];
} t_buffer_chunk;

char *get_next_line(int fd);
#endif

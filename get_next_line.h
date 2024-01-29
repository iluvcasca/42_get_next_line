/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:22:00 by kgriset           #+#    #+#             */
/*   Updated: 2023/11/27 20:24:40 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# include <stdlib.h>
# include <unistd.h>
# define FD_SIZE 500

typedef struct s_buffer_chunk
{
	struct s_buffer_chunk	*next;
	char					buff[BUFFER_SIZE];
}							t_buffer_chunk;

typedef struct s_build_l
{
	size_t					i;
	char					*line;
	size_t					b_remain;
}							t_build_l;

typedef struct s_buff_ctrl
{
	struct s_buffer_chunk	*first_node;
	struct s_buffer_chunk	*node;
	unsigned int			node_counter;
}							t_buff_ctrl;

typedef struct s_nl
{
	unsigned int			is_found;
	size_t					index;
}							t_nl;

char						*get_next_line(int fd);
t_buff_ctrl					add_back(t_buff_ctrl *buff_ctrl);
void						init_t_buff_ctrl(t_buff_ctrl *buff_ctrl);
void						free_t_buffer_chunk(t_buffer_chunk *first);
unsigned int				is_line(char *buffer, t_nl *nl, size_t bytes_read);
char						*extract_remaining_line(char *remain_buffer,
								size_t nl_index);
unsigned int				check_failure(t_buff_ctrl buff_ctrl, int bytes_read,
								size_t len_remain);
char						*build_line(t_buff_ctrl buff_ctrl,
								char *remain_buffer, t_nl nl, int bytes_read);
size_t						gnl_strlen(char *buffer);
size_t						gnl_strlcpy(char *dst, const char *src,
								size_t dstsize);
#endif

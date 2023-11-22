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

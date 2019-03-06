# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsprigga <bsprigga@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/01 17:36:37 by bsprigga          #+#    #+#              #
#    Updated: 2019/03/03 03:10:33 by tsimonis         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re rebuild_lib re1 fclean1

NAME = lem-in

INCLUDES := ./includes ./libft/includes
CC = clang
FLAGS := -Wall -Werror -Wextra -g
VPATH := . srcs

SRCS = lem-in.c read_input.c support_part_1.c

OBJECTS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): libft/libft.a $(OBJECTS)
	gcc $(FLAGS) $(OBJECTS) $(addprefix -I,$(INCLUDES)) -lft -L./libft -o $@

libft/libft.a:
	$(MAKE) -C libft

$(OBJECTS): %.o: %.c
	gcc $(FLAGS) -c $< -o $@ $(addprefix -I,$(INCLUDES))

clean:
	rm -f $(OBJECTS)
	#$(MAKE) -C ./libft clean

fclean: clean
	rm -f $(NAME)

re: fclean all

rebuild_lib:
	$(MAKE) re -C libft

re1: rebuild_lib fclean all

fclean1: fclean
	$(MAKE) fclean -C libft
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jheat <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/31 18:11:19 by jheat             #+#    #+#              #
#    Updated: 2021/01/31 18:11:22 by jheat            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_one

LIST  =	main.c feast.c print_utils.c utils.c

OBJ = $(patsubst %.c,%.o,$(LIST))

HEADER = philosophers.h

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
		clang -Wall -Wextra -Werror -o $(NAME) $(OBJ)

%.o: %.c $(HEADER)
		gcc $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

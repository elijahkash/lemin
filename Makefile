# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/13 16:02:18 by mtrisha           #+#    #+#              #
#    Updated: 2020/01/09 16:21:03 by mtrisha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in

CMAKEDIR = ./tmp/
LIBDIR = ./libft/

all: $(NAME)

$(NAME): callcmake

callcmake: $(CMAKEDIR) $(CMAKEDIR)Makefile
	$(MAKE) -C $(CMAKEDIR) $(MAKECMDGOALS)

$(CMAKEDIR):
	mkdir -p $(CMAKEDIR)

cmake: $(CMAKEDIR)
	cd $(CMAKEDIR); cmake ../

$(CMAKEDIR)Makefile:
	cd $(CMAKEDIR); cmake ../

clean:
	$(MAKE) -C $(LIBDIR) clean
	rm -r -f $(CMAKEDIR)

fclean:
	$(MAKE) -C $(LIBDIR) fclean
	rm -r -f $(CMAKEDIR)
	rm -r -f $(NAME)
	rm -r -f $(NAME).debug

re: fclean
	$(MAKE) all

debug: all


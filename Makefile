# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Kashnitskiy <elijahkash.code@gmail.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/13 16:02:18 by mtrisha           #+#    #+#              #
#    Updated: 2020/01/27 17:03:18 by Kashnitskiy      ###   ########.fr        #
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
	rm -r -f *.dSYM

fclean:
	$(MAKE) -C $(LIBDIR) fclean
	rm -r -f $(CMAKEDIR)
	rm -r -f $(NAME)
	rm -r -f $(NAME).debug
	rm -r -f *.dSYM

re: fclean
	$(MAKE) all

debug: all


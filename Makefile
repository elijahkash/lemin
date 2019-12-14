# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile_cmake                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/13 16:02:18 by mtrisha           #+#    #+#              #
#    Updated: 2019/12/14 13:02:09 by mtrisha          ###   ########.fr        #
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

re: fclean
	$(MAKE) all

debug: all


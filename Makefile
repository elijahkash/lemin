# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtrisha <mtrisha@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/19 15:35:23 by mtrisha           #+#    #+#              #
#    Updated: 2019/11/07 13:23:37 by mtrisha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in
SOURCES = main.c lemin.c t_farm.c handle_input.c solve.c

CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEBUG = -O3
debug: DEBUG = -g3
MKDIR = mkdir -p
CMDGOALS = $(MAKECMDGOALS)

LIBRARY = ft
LIBHEADER = includes/libft.h
LIBDIR = ./libft/
LIB = $(LIBDIR)lib$(LIBRARY).a
ADDLIB = -L $(LIBDIR) -l$(LIBRARY)

SRCDIR = ./sources/
OBJDIR = ./objects/
INCDIR = ./includes/  $(LIBDIR)includes/
DEBDIR = ./debug/

OBJECTS1 = $(SOURCES:.c=.o)
OBJECTS = $(notdir $(OBJECTS1))
SRC = $(addprefix $(SRCDIR), $(SOURCES))
OBJ = $(addprefix $(OBJDIR), $(OBJECTS))
INC = $(addprefix -I , $(INCDIR))

all: depend $(NAME)

$(LIB): buildlib

makelib:
	$(MAKE) -C $(LIBDIR) $(CMDGOALS)

buildlib:
	$(MAKE) -C $(LIBDIR)

depend: $(OBJDIR).depend

$(OBJDIR).depend: $(SRC)
	$(CC) -MM $(SRC) $(INC) > $(OBJDIR).depend
	sed -i.bak  '/.o/s/^/objects\//g' $(OBJDIR).depend
	sed -i.bak  '/.o/s/objects\/  //g' $(OBJDIR).depend
	rm -rf $(OBJDIR).depend.bak

-include $(OBJDIR).depend

$(NAME): $(LIB) $(OBJ)
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $(OBJ) $(INC) $(ADDLIB)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) $(DEBUG) -o $@ -c $< $(INC)

clean: makelib
	rm -rf $(LIB)
	rm -rf *.dSYM
	rm -rf $(OBJ)
	rm -rf $(OBJDIR).depend
	rm -rf $(OBJDIR).depend.bak

fclean: makelib
	rm -r -f $(NAME)
	rm -rf *.dSYM
	rm -rf $(OBJ)
	rm -rf $(OBJDIR).depend
	rm -rf $(OBJDIR).depend.bak

debug: all

re: fclean
	$(MAKE)

redebug: fclean
	$(MAKE) debug

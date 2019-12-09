# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtrisha <mtrisha@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/19 15:35:23 by mtrisha           #+#    #+#              #
#    Updated: 2019/12/09 12:50:21 by mtrisha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in
SOURCES = main.c old_lemin.c old_farm.c old_handle_input.c \
	old_prepare_work_graph.c old_solve.c \
	farm.c

CC = gcc
CFLAGS = -Wall -Wextra #-Werror
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

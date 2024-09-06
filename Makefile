NAME = minishell
COMPIL = cc
FLAGS = -Wall -Werror -Wextra -I .
# -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fsanitize=integer -fsanitize=null -fsanitize=unreachable
#ATTENTION FLAG A UTILISER SEPAREMENT DE VALGRIND
PINK = \033[1;35m
BLUE = \033[1;36m
NC = \033[0m

HEADER_F = headers/minishell.h \
			headers/built_in.h \
			headers/parsing.h \

BUILTIN_SRCS = ft_echo.c \
			ft_env.c \
			ft_pwd.c \
			ft_exit.c \
			ft_exit_utils.c \
			ft_cd.c \
			ft_export.c \
			ft_export_utils.c \
			ft_unset.c \

PARSING_SRCS = error_parsing.c \
				expand_utils.c \
				expand.c \
				init_cmd_list_utils.c \
				init_cmd_list.c \
				init_cmd_redir.c \
				init_cmd_redir_utils.c \
				parsing_utils.c \
				parsing.c \
				quote.c \
				redir.c \
				token.c \
				here_doc/delimiter.c \
				here_doc/here_doc.c \
				here_doc/put_new_value.c \
				here_doc/random_name.c \

MANDATORY_SRCS = main.c \
				init.c \
				malloc_error.c \
				launcher.c \
				signaux/signal.c \
				cmd/lst_cmd.c \
				redir_file/lst_redir_file_utils.c \
				redir_file/lst_redir_file.c \

ENV_SRCS = lst_env.c \
		lst_env_utils.c \
		lst_env_utils_2.c \


EXECUTION_SRCS = pipe_redirection.c \
				operand_redirection_utils.c \
				check_parents_directory.c \
				exec.c \
				command_path.c \
				command_absolute_relative_path.c \
				operand_redirection.c \
				clean_and_free.c \
				error_message.c \
				exec_built_in.c \


# Magic with srcs and objs #

PMANDATORY = $(addprefix srcs/, $(MANDATORY_SRCS))
PBUILTIN = $(addprefix srcs/built_in/, $(BUILTIN_SRCS))
PENV = $(addprefix srcs/env/, $(ENV_SRCS))
PEXECUTION = $(addprefix srcs/execution/, $(EXECUTION_SRCS))
PPARSING = $(addprefix srcs/parsing/, $(PARSING_SRCS))


B_OBJS = $(PBUILTIN:srcs/%.c=objs/%.o)
P_OBJS = $(PPARSING:srcs/%.c=objs/%.o)
M_OBJS = $(PMANDATORY:srcs/%.c=objs/%.o)
E_OBJS = $(PENV:srcs/%.c=objs/%.o)
EX_OBJS = $(PEXECUTION:srcs/%.c=objs/%.o)


LIBFT = libft/libft.a

LDFLAGS = -lreadline

all : $(NAME)

$(LIBFT):
	@make -sC libft
	@make bonus -sC libft
	@echo  "$(PINK)\n     LIBFT COMPILED \n$(NC)"


objs/%.o: srcs/%.c $(HEADER_F)
	@mkdir -p objs/built_in
	@mkdir -p objs/env
	@mkdir -p objs/execution
	@mkdir -p objs/parsing/here_doc
	@mkdir -p objs/cmd
	@mkdir -p objs/redir_file
	@mkdir -p objs/signaux
	@$(COMPIL) $(FLAGS) -c $< -o $@

$(NAME) : $(M_OBJS) $(P_OBJS) $(B_OBJS) $(E_OBJS) $(EX_OBJS)  $(HEADER_F) $(LIBFT)
	@$(COMPIL) $(FLAGS) -o $@ $(M_OBJS) $(P_OBJS) $(B_OBJS) $(E_OBJS) $(EX_OBJS)  $(LIBFT) $(LDFLAGS)
	@echo  "$(BLUE)\n    MINISHELL COMPILED \n$(NC)"

clean :
	@rm -rf objs/*
	@make -sC libft clean
	@echo  "$(PINK) \n     LIBFT CLEANED  \n$(NC)"
	@echo  "$(BLUE) \n     MINISHELL CLEANED \n$(NC)"

fclean : clean
	@rm -f $(NAME)
	@make -sC libft fclean

leak:
	valgrind --suppressions=ignore_leak_readline --trace-children=yes    \
    --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes -q  \
    ./minishell

re : fclean all

.PHONY : all clean fclean re bonus




# clear && make && valgrind --suppressions=ignore_leak_readline --leak-check=full --show-leak-kinds=all  --trace-children=yes ./minishell


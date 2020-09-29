#ifndef STRUCTER_H
#define STRUCTER_H

typedef struct			s_command
{
	char 				**argv;
	int 				pipe;
	int 				redirect;
	int 				file;
	struct s_command	*next;
}						t_command;

t_command	*create_command_lst(void);

#endif
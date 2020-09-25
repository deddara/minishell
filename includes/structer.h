#ifndef STRUCTER_H
#define STRUCTER_H

typedef struct			s_command
{
	char 				**argv;
	int 				pipe;
	int 				redirect;
	struct s_command	*next;
}						t_command;


#endif
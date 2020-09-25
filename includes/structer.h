#ifdef STRUCTER_H
#define STRUCTER_H

typedef struct	s_command;
{
	char 		**argv;
	int 		pipe;
	int 		redirect;
}				t_command;


#endif

# include "minishell.h"

void	ft_pwd(void)
{
	char	*buff;
	int		buff_size;

	buff_size = 42;
	buff = ft_calloc(sizeof(char), buff_size);
	while (getcwd(buff, buff_size) == NULL)
	{
		free(buff);
		buff_size *= 2;
		buff = ft_calloc(sizeof(char), buff_size);
	}
	ft_printf("%s\n", buff);
	free(buff);
}

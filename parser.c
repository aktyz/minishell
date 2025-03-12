#include "minishell.h"

//we have to be sure that everything is 'zero' before using the structure
static void initialize_cmd(t_command **cmd)
{
    *cmd->command = NULL;
    *cmd->path = NULL;
    *cmd->args = NULL;
    *cmd->pipe_output = false;
    *cmd->pipe_fd = 0;
    *cmd->prev = NULL;
    *cmd->next = NULL;    
}
//creating new element of the list t_command

t_command   *lst_new_cmd(bool value)
{
    t_command   *new_node;
    
    new_node = (t_command *)malloc(sizeof(t_command));
    if (!new_node)
        return (NULL);
    ft_memset(new_node, 0, sizeof(t_command));
    new_node->pipe_out(&new_node);
    initialize_cmd(&new_node);
    return (new_node);
}

//adding new element at the end of the list

void    lst_add_back_cmd(t_command **alst, t_command *new_node)
{
    t_command   *start;

    start = *alst;
    if (start == NULL)
    {
        *alst = new_node;
        return;
    }
    if (alst && *alst && new_node)
    {
        while (start->next != NULL)
            start = start->next;
        start->next = new_node;
        new_node-> = start;
    }
}

//returning the last element of the list

t_command   *lst_last_cmd(t_command *cmd)
{
    while (cmd->next != NULL)
        cmd = cmd->next;
    return (cmd);
}

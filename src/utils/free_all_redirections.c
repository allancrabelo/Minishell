#include "minishell.h"

/**
 * @brief Frees redirection linked list and associated resources
 * 
 * Iterates through redirection structures, freeing filenames,
 * heredoc delimiters, and node memory.
 * 
 * @param redir Pointer to the head of the redirection list
 * @return void
 */
void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->file)
			free(redir->file);
		if (redir->heredoc_delimeter)
			free(redir->heredoc_delimeter);
		free(redir);
		redir = tmp;
	}
}

/**
 * @brief Frees all heredoc structures and associated resources
 * 
 * Iterates through the heredoc linked list, freeing delimiters,
 * closing pipe file descriptors, and freeing node memory.
 * 
 * @param head Double pointer to the head of the heredoc list
 * @return void
 */
void	free_heredocs(t_heredoc **head)
{
	t_heredoc	*cur;
	t_heredoc	*tmp;

	if (!head || !*head)
		return ;
	cur = *head;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		if (tmp->heredoc_delimeter)
		{
			free(tmp->heredoc_delimeter);
			tmp->heredoc_delimeter = NULL;
		}
		if (tmp->pipe_fd != -1)
		{
			close(tmp->pipe_fd);
			tmp->pipe_fd = -1;
		}
		free(tmp);
	}
	*head = NULL;
}

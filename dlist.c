#include "dlist.h"

t_dlist	*dlst_add_right(t_dlist **lst, t_dlist *newlst)
{
	if (!lst || !newlst)
		return (NULL);
	if (!*lst)  // 何も入ってなかった
	{
		*lst = newlst;
		(*lst)->next = *lst;
		(*lst)->prev = *lst;
	}
	else
	{
		newlst->next = (*lst)->next;
		newlst->prev = *lst;
		(*lst)->next->prev = newlst;
		(*lst)->next = newlst;
	}
	*lst = newlst;
	return (*lst);
}

t_dlist	*dlst_add_right_new(t_dlist **lst, void *content)
{
	t_dlist *newlst;

	if (!(newlst = dlst_new(content)))
		return (NULL);
	return (dlst_add_right(lst, newlst));
}

t_dlist	*dlst_add_left(t_dlist **lst, t_dlist *newlst)
{
	if (!lst || !newlst)
		return (NULL);
	if (!*lst)  // 何も入ってなかった
	{
		*lst = newlst;
		(*lst)->next = *lst;
		(*lst)->prev = *lst;
	}
	else
	{
		newlst->next = *lst;
		newlst->prev = (*lst)->prev;
		(*lst)->prev->next = newlst;
		(*lst)->prev = newlst;
	}
	*lst = newlst;
	return (*lst);
}

t_dlist	*dlst_add_left_new(t_dlist **lst, void *content)
{
	t_dlist *newlst;

	if (!(newlst = dlst_new(content)))
		return (NULL);
	return (dlst_add_left(lst, newlst));

}

void	dlst_clear(t_dlist **lst, void (*del)(void *))
{
	t_dlist *current;
	t_dlist *first_el;
	t_dlist *next_el;

	if (!lst || !*lst)
		return ;
	first_el = *lst;
	current = *lst;
	while (current)
	{
		next_el = current->next;
		dlst_delone(current, del);
		current = next_el;
		if (current == first_el)
			break;
	}
	*lst = NULL;
}

void	dlst_delone(t_dlist *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	if (del)
		del(lst->content);
	free(lst);
}

t_dlist	*dlst_new(void *content)
{
	t_dlist	*newlst;

	if (!(newlst = malloc(sizeof(t_dlist))))
		return (NULL);
	newlst->content = content;
	newlst->next = NULL;
	newlst->prev = NULL;
	return (newlst);
}

void	print_dlst(t_dlist *lst)
{
	t_dlist *head;

	if (!lst)
		return;
	head = lst;
	do {
		printf("addr: %p\n", lst);
		printf("\tcontent: %p\n", lst->content);
		printf("\tprev: %p\n", lst->prev);
		printf("\tnext: %p\n", lst->next);
		lst = lst->next;
	}while (lst != head);
}

#ifndef DLIST_H
# define DLIST_H
#include "stdlib.h"
#include "stdio.h"

typedef struct	s_dlist {
	void			*content;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}				t_dlist;

t_dlist		*dlst_add_right(t_dlist **lst, t_dlist *newlst); // 現在指す要素の後ろ(prev)に挿入
t_dlist		*dlst_add_right_new(t_dlist **lst, void *content);
t_dlist		*dlst_add_left(t_dlist **lst, t_dlist *newlst); // 現在指す要素の前(next)に挿入
t_dlist		*dlst_add_left_new(t_dlist **lst, void *content);
size_t		dlst_size(t_dlist *t_dlist);
void		dlst_clear(t_dlist **lst, void (*del)(void *));
void		dlst_delone(t_dlist *lst, void (*del)(void *));
t_dlist		*dlst_new(void *content);
void		print_dlst(t_dlist *lst);

#endif

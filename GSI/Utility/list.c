
#include "list.h"

inline void list_init(list_t *l)
{
    l->next = l->prev = l;
}
 
inline void list_inseafter(list_t *l, list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

inline void list_insebefore(list_t *l, list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}
 
inline void list_remove(list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}
 
inline int list_isempty(const list_t *l)
{
    return l->next == l;
}

inline unsigned int list_len(const list_t *l)
{
    unsigned int len = 0;
    const list_t *p = l;
    while (p->next != l)
    {
        p = p->next;
        len ++;
    }

    return len;
}




/**
 * @brief initialize a single list
 *
 * @param l the single list to be initialized
 */
inline void slist_init(slist_t *l)
{
    l->next = GSI_NULL;
}
 
inline void slist_append(slist_t *l, slist_t *n)
{
    struct slist_node *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next = GSI_NULL;
}
 
inline void slist_insert(slist_t *l, slist_t *n)
{
    n->next = l->next;
    l->next = n;
}
 
inline unsigned int slist_len(const slist_t *l)
{
    unsigned int len = 0;
    const slist_t *list = l->next;
    while (list != GSI_NULL)
    {
        list = list->next;
        len ++;
    }

    return len;
}
 
inline slist_t *slist_remove(slist_t *l, slist_t *n)
{
    /* remove slist head */
    struct slist_node *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (slist_t *)0) node->next = node->next->next;

    return l;
}
 
inline slist_t *slist_first(slist_t *l)
{
    return l->next;
}
 
inline slist_t *slist_tail(slist_t *l)
{
    while (l->next) l = l->next;

    return l;
}
 
inline slist_t *slist_next(slist_t *n)
{
    return n->next;
}
 
inline int slist_isempty(slist_t *l)
{
    return l->next == GSI_NULL;
}


#ifndef LIST_H
#define LIST_H

#include  "gsi_def.h"
 
/*Double List structure*/
struct list_node
{
    struct list_node *next;                          
    struct list_node *prev;                         
};
typedef struct list_node  list_t;                   

/*Single List structure*/
struct slist_node
{
    struct slist_node *next;                          
};
typedef struct slist_node slist_t;          

      
inline void list_init(list_t *l);
inline void list_inseafter(list_t *l, list_t *n);
inline void list_insebefore(list_t *l, list_t *n);
inline void list_remove(list_t *n);
inline int list_isempty(const list_t *l);
inline unsigned int list_len(const list_t *l);

inline void slist_init(slist_t *l);
inline void slist_append(slist_t *l, slist_t *n);
inline void slist_insert(slist_t *l, slist_t *n);
inline unsigned int slist_len(const slist_t *l);
inline slist_t *slist_remove(slist_t *l, slist_t *n);
inline slist_t *slist_first(slist_t *l);
inline slist_t *slist_tail(slist_t *l);
inline slist_t *slist_next(slist_t *n);
inline int slist_isempty(slist_t *l);
      
      
#endif
#include <string.h>
#include <stdlib.h>
#include "codenode.h"
#include "defines.h"

/**
 * @brief Unlink a child from its parent.
 * @param[in] parent Parent node.
 * @param[in] child Child node.
 */
static void _code_node_unlink_child(code_node_t *parent, code_node_t *child)
{
    size_t i;
    for (i = 0; i < parent->child_sz; i++)
    {
        if (parent->childs[i] == child)
        {
            if (i < parent->child_sz - 1)
            {
                memmove(parent->childs + i, parent->childs + i + 1, sizeof(code_node_t *) * (parent->child_sz - i - 1));
            }

            parent->childs[parent->child_sz - 1] = NULL;
            parent->child_sz--;

            return;
        }
    }
}

void code_node_delete(code_node_t *node)
{
    /* Remove from parent. */
    if (node->parent != NULL)
    {
        _code_node_unlink_child(node->parent, node);
        node->parent = NULL;
    }

    /* Remove all children. */
    while (node->child_sz != 0)
    {
        code_node_delete(node->childs[0]);
    }

    /* Release this node. */
    csplice_string_free(&node->data);
    free(node);
}

void code_node_append_file(code_node_t* node, const char* path)
{
    code_node_t* new_node = calloc(1, sizeof(code_node_t));
    if (new_node == NULL)
    {
        abort();
    }

    new_node->parent = node;
    new_node->child_sz = 0;
    new_node->childs = NULL;
    new_node->type = CODE_NODE_TYPE_FILE;
    csplice_string_set_cstring(&new_node->data, path);

    code_node_t** new_childs = realloc(node->childs, sizeof(code_node_t *) * (node->child_sz + 1));
    if (new_childs == NULL)
    {
        abort();
    }

    node->childs = new_childs;
    node->childs[node->child_sz++] = new_node;
}

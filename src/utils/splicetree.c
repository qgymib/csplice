#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "function/__init__.h"
#include "splicetree.h"
#include "defines.h"

/**
 * @brief Unlink a child from its parent.
 * @param[in] parent Parent node.
 * @param[in] child Child node.
 */
static void _code_node_unlink_child(splice_node_t *parent, splice_node_t *child)
{
    size_t i;
    for (i = 0; i < parent->child_sz; i++)
    {
        if (parent->childs[i] == child)
        {
            if (i < parent->child_sz - 1)
            {
                memmove(parent->childs + i, parent->childs + i + 1,
                        sizeof(splice_node_t *) * (parent->child_sz - i - 1));
            }

            parent->childs[parent->child_sz - 1] = NULL;
            parent->child_sz--;

            return;
        }
    }
}

void splice_node_delete(splice_node_t *node)
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
        splice_node_delete(node->childs[0]);
    }

    /* Release this node. */
    if (node->type == SPLICE_NODE_TYPE_FILE)
    {
        csplice_string_free(&node->data.file.path);
        csplice_string_free(&node->data.file.data);
    }
    free(node);
}

int splice_node_append_file(lua_State *L, splice_node_t *node, const char *path)
{
    splice_node_t *new_node = calloc(1, sizeof(splice_node_t));
    if (new_node == NULL)
    {
        return luaL_error(L, "out of memory.");
    }

    new_node->parent = node;
    new_node->child_sz = 0;
    new_node->childs = NULL;
    new_node->type = SPLICE_NODE_TYPE_FILE;

    splice_node_t **new_childs = realloc(node->childs, sizeof(splice_node_t *) * (node->child_sz + 1));
    if (new_childs == NULL)
    {
        return luaL_error(L, "out of memory.");
    }
    node->childs = new_childs;
    node->childs[node->child_sz++] = new_node;

    csplice_string_set_cstring(&new_node->data.file.path, path);

    csplice_get_function(L, "readfile"); // sp+1
    lua_pushstring(L, path);             // sp+2
    lua_call(L, 1, 1);                   // sp+1
    csplice_string_set_lstring(&new_node->data.file.data, L, -1);
    lua_pop(L, 1); // sp+0

    return 0;
}

void splice_node_to_json(cJSON *dst, const splice_node_t *node)
{
    if (node->type == SPLICE_NODE_TYPE_VIRT)
    {
        cJSON *virt = cJSON_CreateArray();
        cJSON_AddItemToObject(dst, "virt", virt);
        for (size_t i = 0; i < node->child_sz; i++)
        {
            splice_node_to_json(virt, node->childs[i]);
        }
        return;
    }

    cJSON *obj = cJSON_CreateObject();
    if (node->type == SPLICE_NODE_TYPE_FILE)
    {
        cJSON_AddStringToObject(obj, "data", node->data.file.data.str);
        cJSON_AddStringToObject(obj, "path", node->data.file.path.str);
    }
    cJSON_AddItemToArray(dst, obj);
}

splice_node_t* splice_tree_begin(splice_node_t* root)
{
    while (root->child_sz != 0)
    {
        root = root->childs[0];
    }
    return root;
}

splice_node_t* splice_tree_next(splice_node_t* node)
{
    splice_node_t* parent = node->parent;
    if (parent == NULL)
    {
        return NULL;
    }

    size_t pos;
    for (pos = 0; pos < parent->child_sz; pos++)
    {
        if (parent->childs[pos] == node)
        {
            /* If we have brothers. */
            if (pos < parent->child_sz - 1)
            {
                return splice_tree_begin(parent->childs[pos + 1]);
            }

            /* If no more brothers, return parent. */
            return parent;
        }
    }
    /* Should never happen. */
    assert(pos < parent->child_sz);
    return NULL;
}

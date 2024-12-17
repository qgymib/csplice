#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "function/__init__.h"
#include "utils/defines.h"
#include "utils/readfile.h"
#include "__init__.h"

/**
 * @brief Unlink a child from its parent.
 * @param[in] parent Parent node.
 * @param[in] child Child node.
 */
static void _code_node_unlink_child(pass_node_t *parent, pass_node_t *child)
{
    size_t i;
    for (i = 0; i < parent->child_sz; i++)
    {
        if (parent->childs[i] == child)
        {
            if (i < parent->child_sz - 1)
            {
                memmove(parent->childs + i, parent->childs + i + 1,
                        sizeof(pass_node_t *) * (parent->child_sz - i - 1));
            }

            parent->childs[parent->child_sz - 1] = NULL;
            parent->child_sz--;

            return;
        }
    }
}

pass_node_t* pass_node_new(void)
{
    pass_node_t* node = calloc(1, sizeof(pass_node_t));
    if (node == NULL)
    {
        abort();
    }
    return node;
}

static void _pass_node_to_virt(pass_node_t *node)
{
    if (node->type == PASS_NODE_TYPE_FILE)
    {
        csplice_string_free(&node->data.file.path);
        csplice_string_free(&node->data.file.data);
    }
    node->type = PASS_NODE_TYPE_VIRT;
}

void pass_tree_delete(pass_node_t *node)
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
        pass_tree_delete(node->childs[0]);
    }

    /* Release this node. */
    _pass_node_to_virt(node);
    free(node);
}

int pass_node_set_file(pass_node_t *node, const char *path)
{
    _pass_node_to_virt(node);
    node->type = PASS_NODE_TYPE_FILE;
    csplice_string_set_cstring(&node->data.file.path, path);
    return csplice_readfile(&node->data.file.data, path);
}

void pass_node_file_append_search_path(pass_node_t* node, const char* path, int iquote)
{
    csplice_string_t** p_i = &node->data.file.isystem;
    size_t* p_s = &node->data.file.isystem_sz;
    if (iquote)
    {
        p_i = &node->data.file.iquote;
        p_s = &node->data.file.iquote_sz;
    }

    size_t old_sz = *p_s;
    size_t new_sz = old_sz + 1;
    csplice_string_t* new_arr = realloc(*p_i, sizeof(csplice_string_t) * new_sz);
    if (new_arr == NULL)
    {
        abort();
    }
    *p_i = new_arr;
    
    new_arr[old_sz].str = NULL;
    new_arr[old_sz].len = 0;

    csplice_string_set_cstring(&new_arr[old_sz], path);
    *p_s = new_sz;
}

void pass_node_insert(pass_node_t* parent, pass_node_t* child, size_t pos)
{
    pass_node_t **new_childs = realloc(parent->childs, sizeof(pass_node_t *) * (parent->child_sz + 1));
    if (new_childs == NULL)
    {
        abort();
    }
    parent->childs = new_childs;

    if (pos < parent->child_sz)
    {
        memmove(parent->childs + pos + 1, parent->childs + pos,
            sizeof(pass_node_t *) * (parent->child_sz - pos));
    }
    else
    {
        pos = parent->child_sz;
    }

    parent->childs[pos] = child;
    parent->child_sz++;
    child->parent = parent;
}

void pass_tree_to_json(cJSON *dst, const pass_node_t *node)
{
    size_t i;
    if (node->type == PASS_NODE_TYPE_VIRT)
    {
        cJSON *virt = cJSON_CreateArray();
        cJSON_AddItemToObject(dst, "virt", virt);
        for (size_t i = 0; i < node->child_sz; i++)
        {
            pass_tree_to_json(virt, node->childs[i]);
        }
        return;
    }

    cJSON *obj = cJSON_CreateObject();
    if (node->type == PASS_NODE_TYPE_FILE)
    {
        cJSON_AddStringToObject(obj, "type", "file");
        cJSON_AddStringToObject(obj, "data", node->data.file.data.str);
        cJSON_AddStringToObject(obj, "path", node->data.file.path.str);

        cJSON* arr = cJSON_CreateArray();
        for (i = 0; i < node->data.file.iquote_sz; i++)
        {
            cJSON_AddItemToArray(arr, cJSON_CreateString(node->data.file.iquote[i].str));
        }
        cJSON_AddItemToObject(obj, "iquote", arr);

        arr = cJSON_CreateArray();
        for (i = 0; i < node->data.file.isystem_sz; i++)
        {
            cJSON_AddItemToArray(arr, cJSON_CreateString(node->data.file.isystem[i].str));
        }
        cJSON_AddItemToObject(obj, "isystem", arr);
    }
    cJSON_AddItemToArray(dst, obj);
}

pass_node_t* pass_tree_begin(pass_node_t* root)
{
    while (root->child_sz != 0)
    {
        root = root->childs[0];
    }
    return root;
}

pass_node_t* pass_node_next(pass_node_t* node)
{
    pass_node_t* parent = node->parent;
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
                return pass_tree_begin(parent->childs[pos + 1]);
            }

            /* If no more brothers, return parent. */
            return parent;
        }
    }
    /* Should never happen. */
    assert(pos < parent->child_sz);
    return NULL;
}


#ifndef __CSPLICE_UTILS_CODENODE_H__
#define __CSPLICE_UTILS_CODENODE_H__

#include <stddef.h>
#include "str.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum code_node_type
{
    /**
     * @brief A virtual node that only contains child nodes.
     */
    CODE_NODE_TYPE_VIRT = 0,

    /**
     * @brief A file node.
     * A file node contains a path to a file.
     * A file node has no child nodes.
     */
    CODE_NODE_TYPE_FILE,

    /**
     * @brief A text node.
     * A text node contains text.
     * A text node has no child nodes.
     */
    CODE_NODE_TYPE_TEXT,

    /**
     * @brief A code node.
     * A code node contains code.
     * A code node has no child nodes.
     */
    CODE_NODE_TYPE_CODE,
} code_node_type_t;

/**
 * @brief Structure to represent a code node.
 */
typedef struct code_node
{
    struct code_node  *parent;   /**< Parent node */
    struct code_node **childs;   /**< Child nodes */
    size_t             child_sz; /**< Number of child nodes */
    code_node_type_t   type;     /**< Type of code node */
    csplice_string_t   data;     /**< Data */
} code_node_t;

/**
 * @brief Delete a node and it's child nodes.
 * @param[in] node Node to delete.
 */
void code_node_delete(code_node_t *node);

/**
 * @brief Append child nodes into \p parent.
 * @param[in] parent Parent node.
 * @param[in] path  Path to file.
 */
void code_node_append_file(code_node_t *parent, const char *path);

#ifdef __cplusplus
}
#endif
#endif

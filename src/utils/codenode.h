#ifndef __CSPLICE_UTILS_CODENODE_H__
#define __CSPLICE_UTILS_CODENODE_H__

#include <stddef.h>
#include "str.h"
#include "luaext.h"

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
     * @brief A code node.
     * A code node contains code.
     * A code node has no child nodes.
     */
    CODE_NODE_TYPE_CODE,
} code_node_type_t;

typedef struct code_node_file
{
    csplice_string_t path;  /**< Path to file */
    csplice_string_t data;  /**< File content */
} code_node_file_t;

/**
 * @brief Structure to represent a code node.
 */
typedef struct code_node
{
    struct code_node  *parent;   /**< Parent node */
    struct code_node **childs;   /**< Child nodes */
    size_t             child_sz; /**< Number of child nodes */
    code_node_type_t   type;     /**< Type of code node */

    union {
        code_node_file_t file;
    } data;
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
 * @return Always 0.
 */
int code_node_append_file(lua_State *L, code_node_t *parent, const char *path);

#ifdef __cplusplus
}
#endif
#endif

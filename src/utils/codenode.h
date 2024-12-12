#ifndef __CSPLICE_UTILS_CODENODE_H__
#define __CSPLICE_UTILS_CODENODE_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum code_node_type
{
    /**
     * @brief Root node.
     * A root node has no parent.
     */
    CODE_NODE_TYPE_ROOT = 0,

    CODE_NODE_TYPE_FILE,
} code_node_type_t;

typedef struct code_node_file
{
    char *path; /**< Path to file */
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

void code_node_append_file(code_node_t* node, const char* path);

#ifdef __cplusplus
}
#endif
#endif

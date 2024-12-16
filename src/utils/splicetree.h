#ifndef __CSPLICE_UTILS_SPLICE_TREE_H__
#define __CSPLICE_UTILS_SPLICE_TREE_H__

#include <stddef.h>
#include <cJSON.h>
#include "str.h"
#include "luaext.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum splice_node_type
{
    /**
     * @brief A virtual node that only contains child nodes.
     */
    SPLICE_NODE_TYPE_VIRT = 0,

    /**
     * @brief A file node.
     * A file node contains a path to a file.
     */
    SPLICE_NODE_TYPE_FILE,
} splice_node_type_t;

/**
 * @brief A file node.
 * A file node store the file path and it's raw content.
 * If futher processing is performed, the result is stored in child nodes.
 */
typedef struct splice_node_file
{
    csplice_string_t path; /**< Path to file */
    csplice_string_t data; /**< File content */
} splice_node_file_t;

/**
 * @brief Structure to represent a code node.
 */
typedef struct splice_node
{
    struct splice_node  *parent;   /**< Parent node */
    struct splice_node **childs;   /**< Child nodes */
    size_t               child_sz; /**< Number of child nodes */
    splice_node_type_t   type;     /**< Type of code node */

    union {
        splice_node_file_t file;
    } data;
} splice_node_t;

/**
 * @brief Delete a node and it's child nodes.
 * @param[in] node Node to delete.
 */
void splice_node_delete(splice_node_t *node);

/**
 * @brief Dump tree as json into \p dst.
 * @param[out] dst JSON object.
 * @param[in] node Code tree.
 */
void splice_node_to_json(cJSON* dst, const splice_node_t* node);

/**
 * @brief Get the first child node of \p root.
 * @param[in] root Root node.
 * @return First child node
 */
splice_node_t* splice_tree_begin(splice_node_t* root);

/**
 * @brief Get the next node of \p node.
 * @param[in] node Current node.
 * @return Next node
 */
splice_node_t* splice_tree_next(splice_node_t* node);

/**
 * @brief Append child nodes into \p parent.
 * @param[in] parent Parent node.
 * @param[in] path  Path to file.
 * @return Always 0.
 */
int splice_node_append_file(lua_State *L, splice_node_t *parent, const char *path);

#ifdef __cplusplus
}
#endif
#endif

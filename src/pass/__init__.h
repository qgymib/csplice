#ifndef __CSPLICE_PASS_INIT_H__
#define __CSPLICE_PASS_INIT_H__

#include <stddef.h>
#include <cJSON.h>
#include "utils/str.h"
#include "utils/luaext.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum passs_node_type
{
    /**
     * @brief A virtual node that only contains child nodes.
     */
    PASS_NODE_TYPE_VIRT = 0,

    /**
     * @brief A file node.
     * A file node contains a path to a file.
     */
    PASS_NODE_TYPE_FILE,
} pass_node_type_t;

/**
 * @brief A file node.
 * A file node store the file path and it's raw content.
 * If futher processing is performed, the result is stored in child nodes.
 */
typedef struct pass_node_file
{
    csplice_string_t *iquote; /**< Search pathes. Apply to #include "file". */
    size_t            iquote_sz;

    csplice_string_t *isystem; /**< Search pathes. Apply to #include "file" and #include <file>. */
    size_t            isystem_sz;

    csplice_string_t path; /**< Path to file */
    csplice_string_t data; /**< File content */
} pass_node_file_t;

/**
 * @brief Structure to represent a code node.
 */
typedef struct pass_node
{
    struct pass_node  *parent;   /**< Parent node */
    struct pass_node **childs;   /**< Child nodes */
    size_t             child_sz; /**< Number of child nodes */
    pass_node_type_t   type;     /**< Type of code node */

    union {
        pass_node_file_t file;
    } data;
} pass_node_t;

/**
 * @brief Create a new node.
 * @return New node
 */
pass_node_t *pass_node_new(void);

/**
 * @brief Set a file to a node.
 * @param[in] node Node to set.
 * @param[in] path Path to file.
 * @return 0 if success.
 */
int pass_node_set_file(pass_node_t *node, const char *path);

/**
 * @brief Append search path to file node.
 * @param[in] node File node.
 * @param[in] path Search path.
 * @param[in] iquote If true, apply to #include "file".
 */
void pass_node_file_append_search_path(pass_node_t* node, const char* path, int iquote);

/**
 * @brief Link a child node to a parent node.
 * @param[in] parent Parent node.
 * @param[in] child Child node.
 * @param[in] pos Position of child node. If a child node exists at this position,
 *   elements after it will be moved.
 */
void pass_node_insert(pass_node_t *parent, pass_node_t *child, size_t pos);

/**
 * @brief Delete a node and it's child nodes.
 * @param[in] node Node to delete.
 */
void pass_tree_delete(pass_node_t *node);

/**
 * @brief Dump tree as json into \p dst.
 * @param[out] dst JSON object.
 * @param[in] node Code tree.
 */
void pass_tree_to_json(cJSON *dst, const pass_node_t *node);

/**
 * @brief Get the first child node of \p root.
 * @param[in] root Root node.
 * @return First child node
 */
pass_node_t *pass_tree_begin(pass_node_t *root);

/**
 * @brief Get the next node of \p node.
 * @param[in] node Current node.
 * @return Next node
 */
pass_node_t *pass_node_next(pass_node_t *node);

#ifdef __cplusplus
}
#endif
#endif

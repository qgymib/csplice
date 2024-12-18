#ifndef __CSPLICE_BUILDER_JSON_COMPILATION_DATABASE_H__
#define __CSPLICE_BUILDER_JSON_COMPILATION_DATABASE_H__

#include "pass/__init__.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Build tree from compilation database.
 * @param[in] L Lua VM.
 * @return Always 1.
 * @see https://clang.llvm.org/docs/JSONCompilationDatabase.html
 */
int csplice_build_tree_from_compilation_database(lua_State *L,
    const char* path, pass_node_t* root);

#ifdef __cplusplus
}
#endif
#endif

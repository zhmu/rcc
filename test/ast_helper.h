#pragma once

#include <string>
#include "ast.h"

std::string format_ast_node(struct AST_NODE* n);
std::string ast_to_string(struct AST_NODE* n);

void dump_ast(struct AST_NODE* n);

/*
 * Expression Abstract Syntax Tree Functions
 *
 * Copyright 2002 Ove Kaaven
 * Copyright 2006-2008 Robert Shearman
 * Copyright 2009 Jokul for Tranzda
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "widl.h"
#include "utils.h"
#include "expr.h"
#include "header.h"

expr_t *make_expr(enum expr_type type)
{
    expr_t *e = (expr_t *)xmalloc(sizeof(expr_t));
    e->type = type;
    e->ref = NULL;
    e->u.lval = 0;
    e->is_const = FALSE;
    e->cval = 0;
    return e;
}

expr_t *make_exprl(enum expr_type type, long val)
{
    expr_t *e = (expr_t *)xmalloc(sizeof(expr_t));
    e->type = type;
    e->ref = NULL;
    e->u.lval = val;
    e->is_const = FALSE;
    /* check for numeric constant */
    if (type == EXPR_NUM || type == EXPR_HEXNUM || type == EXPR_TRUEFALSE)
    {
        /* make sure true/false value is valid */
        assert(type != EXPR_TRUEFALSE || val == 0 || val == 1);
        e->is_const = TRUE;
        e->cval = val;
    }
    return e;
}

expr_t *make_exprd(enum expr_type type, double val)
{
    expr_t *e = (expr_t *)xmalloc(sizeof(expr_t));
    e->type = type;
    e->ref = NULL;
    e->u.dval = val;
    e->is_const = TRUE;
    e->cval = val;
    return e;
}

expr_t *make_exprs(enum expr_type type, char *val)
{
    expr_t *e;
    e = (expr_t *)xmalloc(sizeof(expr_t));
    e->type = type;
    e->ref = NULL;
    e->u.sval = val;
    e->is_const = FALSE;
    /* check for predefined constants */
    if (type == EXPR_IDENTIFIER)
    {
        var_t *c = find_const(val, 0);
        if (c)
        {
            e->u.sval = c->name;
            free(val);
            e->is_const = TRUE;
            e->cval = c->eval->cval;
        }
    }
    return e;
}

expr_t *make_exprt(enum expr_type type, type_t *tref, expr_t *expr)
{
    expr_t *e;
    e = (expr_t *)xmalloc(sizeof(expr_t));
    e->type = type;
    e->ref = expr;
    e->u.tref = tref;
    e->is_const = FALSE;
    /* check for cast of constant expression */
    if (type == EXPR_SIZEOF)
    {
        switch (tref->type)
        {
        case RPC_FC_BYTE:
        case RPC_FC_CHAR:
        case RPC_FC_SMALL:
        case RPC_FC_USMALL:
            e->is_const = TRUE;
            e->cval = 1;
            break;
        case RPC_FC_WCHAR:
        case RPC_FC_USHORT:
        case RPC_FC_SHORT:
            e->is_const = TRUE;
            e->cval = 2;
            break;
        case RPC_FC_LONG:
        case RPC_FC_ULONG:
        case RPC_FC_FLOAT:
        case RPC_FC_ERROR_STATUS_T:
            e->is_const = TRUE;
            e->cval = 4;
            break;
        case RPC_FC_HYPER:
        case RPC_FC_DOUBLE:
            e->is_const = TRUE;
            e->cval = 8;
            break;
        }
    }
    if (type == EXPR_CAST && expr->is_const)
    {
        e->is_const = TRUE;
        e->cval = expr->cval;
    }
    return e;
}

expr_t *make_expr1(enum expr_type type, expr_t *expr)
{
    expr_t *e;
    e = (expr_t *)xmalloc(sizeof(expr_t));
    e->type = type;
    e->ref = expr;
    e->u.lval = 0;
    e->is_const = FALSE;
    /* check for compile-time optimization */
    if (expr->is_const)
    {
        e->is_const = TRUE;
        switch (type)
        {
        case EXPR_LOGNOT:
            e->cval = !expr->cval;
            break;
        case EXPR_POS:
            e->cval = +expr->cval;
            break;
        case EXPR_NEG:
            e->cval = -expr->cval;
            break;
        case EXPR_NOT:
            e->cval = ~expr->cval;
            break;
        default:
            e->is_const = FALSE;
            break;
        }
    }
    return e;
}

expr_t *make_expr2(enum expr_type type, expr_t *expr1, expr_t *expr2)
{
    expr_t *e;
    e = (expr_t *)xmalloc(sizeof(expr_t));
    e->type = type;
    e->ref = expr1;
    e->u.ext = expr2;
    e->is_const = FALSE;
    /* check for compile-time optimization */
    if (expr1->is_const && expr2->is_const)
    {
        e->is_const = TRUE;
        switch (type)
        {
        case EXPR_ADD:
            e->cval = expr1->cval + expr2->cval;
            break;
        case EXPR_SUB:
            e->cval = expr1->cval - expr2->cval;
            break;
        case EXPR_MOD:
            if (expr2->cval == 0)
            {
                error_loc("divide by zero in expression\n");
                e->cval = 0;
            }
            else
                e->cval = expr1->cval % expr2->cval;
            break;
        case EXPR_MUL:
            e->cval = expr1->cval * expr2->cval;
            break;
        case EXPR_DIV:
            if (expr2->cval == 0)
            {
                error_loc("divide by zero in expression\n");
                e->cval = 0;
            }
            else
                e->cval = expr1->cval / expr2->cval;
            break;
        case EXPR_OR:
            e->cval = expr1->cval | expr2->cval;
            break;
        case EXPR_AND:
            e->cval = expr1->cval & expr2->cval;
            break;
        case EXPR_SHL:
            e->cval = expr1->cval << expr2->cval;
            break;
        case EXPR_SHR:
            e->cval = expr1->cval >> expr2->cval;
            break;
        case EXPR_LOGOR:
            e->cval = expr1->cval || expr2->cval;
            break;
        case EXPR_LOGAND:
            e->cval = expr1->cval && expr2->cval;
            break;
        case EXPR_XOR:
            e->cval = expr1->cval ^ expr2->cval;
            break;
        case EXPR_EQUALITY:
            e->cval = expr1->cval == expr2->cval;
            break;
        case EXPR_INEQUALITY:
            e->cval = expr1->cval != expr2->cval;
            break;
        case EXPR_GTR:
            e->cval = expr1->cval > expr2->cval;
            break;
        case EXPR_LESS:
            e->cval = expr1->cval < expr2->cval;
            break;
        case EXPR_GTREQL:
            e->cval = expr1->cval >= expr2->cval;
            break;
        case EXPR_LESSEQL:
            e->cval = expr1->cval <= expr2->cval;
            break;
        default:
            e->is_const = FALSE;
            break;
        }
    }
    return e;
}

expr_t *make_expr3(enum expr_type type, expr_t *expr1, expr_t *expr2, expr_t *expr3)
{
    expr_t *e;
    e = (expr_t *)xmalloc(sizeof(expr_t));
    e->type = type;
    e->ref = expr1;
    e->u.ext = expr2;
    e->ext2 = expr3;
    e->is_const = FALSE;
    /* check for compile-time optimization */
    if (expr1->is_const && expr2->is_const && expr3->is_const)
    {
        e->is_const = TRUE;
        switch (type)
        {
        case EXPR_COND:
            e->cval = expr1->cval ? expr2->cval : expr3->cval;
            break;
        default:
            e->is_const = FALSE;
            break;
        }
    }
    return e;
}

struct expression_type
{
    int is_variable; /* is the expression resolved to a variable? */
    int is_temporary; /* should the type be freed? */
    type_t *type;
};

static int is_integer_type(const type_t *type)
{
    switch (type->type)
    {
    case RPC_FC_BYTE:
    case RPC_FC_CHAR:
    case RPC_FC_SMALL:
    case RPC_FC_USMALL:
    case RPC_FC_WCHAR:
    case RPC_FC_SHORT:
    case RPC_FC_USHORT:
    case RPC_FC_LONG:
    case RPC_FC_ULONG:
    case RPC_FC_INT3264:
    case RPC_FC_UINT3264:
    case RPC_FC_HYPER:
    case RPC_FC_ENUM16:
    case RPC_FC_ENUM32:
        return TRUE;
    default:
        return FALSE;
    }
}

static void check_scalar_type(const struct expr_loc *expr_loc,
                              const type_t *cont_type, const type_t *type)
{
    if (!cont_type || (!is_integer_type(type) && !is_ptr(type) &&
                       type->type != RPC_FC_FLOAT &&
                       type->type != RPC_FC_DOUBLE))
        error_loc_info(&expr_loc->v->loc_info, "scalar type required in expression%s%s\n",
                       expr_loc->attr ? " for attribute " : "",
                       expr_loc->attr ? expr_loc->attr : "");
}

static void check_arithmetic_type(const struct expr_loc *expr_loc,
                                  const type_t *cont_type, const type_t *type)
{
    if (!cont_type || (!is_integer_type(type) &&
                       type->type != RPC_FC_FLOAT &&
                       type->type != RPC_FC_DOUBLE))
        error_loc_info(&expr_loc->v->loc_info, "arithmetic type required in expression%s%s\n",
                       expr_loc->attr ? " for attribute " : "",
                       expr_loc->attr ? expr_loc->attr : "");
}

static void check_integer_type(const struct expr_loc *expr_loc,
                               const type_t *cont_type, const type_t *type)
{
    if (!cont_type || !is_integer_type(type))
        error_loc_info(&expr_loc->v->loc_info, "integer type required in expression%s%s\n",
                       expr_loc->attr ? " for attribute " : "",
                       expr_loc->attr ? expr_loc->attr : "");
}

static type_t *find_identifier(const char *identifier, const type_t *cont_type, int *found_in_cont_type)
{
    type_t *type = NULL;
    const var_t *field;
    const var_list_t *fields = NULL;

    *found_in_cont_type = 0;

    if (cont_type && (cont_type->type == RPC_FC_FUNCTION || is_struct(cont_type->type)))
        fields = cont_type->fields_or_args;
    else if (cont_type && is_union(cont_type->type))
    {
        if (cont_type->type == RPC_FC_ENCAPSULATED_UNION)
        {
            const var_t *uv = LIST_ENTRY(list_tail(cont_type->fields_or_args), const var_t, entry);
            fields = uv->type->fields_or_args;
        }
        else
            fields = cont_type->fields_or_args;
    }

    if (fields) LIST_FOR_EACH_ENTRY( field, fields, const var_t, entry )
        if (field->name && !strcmp(identifier, field->name))
        {
            type = field->type;
            *found_in_cont_type = 1;
            break;
        }

    if (!type)
    {
        var_t *const_var = find_const(identifier, 0);
        if (const_var) type = const_var->type;
    }

    return type;
}

static struct expression_type resolve_expression(const struct expr_loc *expr_loc,
                                                 const type_t *cont_type,
                                                 const expr_t *e)
{
    struct expression_type result;
    result.is_variable = FALSE;
    result.is_temporary = FALSE;
    result.type = NULL;
    switch (e->type)
    {
    case EXPR_VOID:
        break;
    case EXPR_HEXNUM:
    case EXPR_NUM:
    case EXPR_TRUEFALSE:
        result.is_variable = FALSE;
        result.is_temporary = FALSE;
        result.type = find_type("int", 0);
        break;
    case EXPR_STRLIT:
        result.is_variable = FALSE;
        result.is_temporary = TRUE;
        result.type = make_type(RPC_FC_RP, find_type("char", 0));
        break;
    case EXPR_WSTRLIT:
        result.is_variable = FALSE;
        result.is_temporary = TRUE;
        result.type = make_type(RPC_FC_RP, find_type("wchar_t", 0));
        break;
    case EXPR_DOUBLE:
        result.is_variable = FALSE;
        result.is_temporary = FALSE;
        result.type = find_type("double", 0);
        break;
    case EXPR_IDENTIFIER:
    {
        int found_in_cont_type;
        result.is_variable = TRUE;
        result.is_temporary = FALSE;
        result.type = find_identifier(e->u.sval, cont_type, &found_in_cont_type);
        if (!result.type)
        {
            error_loc_info(&expr_loc->v->loc_info, "identifier %s cannot be resolved in expression%s%s\n",
                           e->u.sval, expr_loc->attr ? " for attribute " : "",
                           expr_loc->attr ? expr_loc->attr : "");
        }
        break;
    }
    case EXPR_LOGNOT:
        result = resolve_expression(expr_loc, cont_type, e->ref);
        check_scalar_type(expr_loc, cont_type, result.type);
        result.is_variable = FALSE;
        result.is_temporary = FALSE;
        result.type = find_type("int", 0);
        break;
    case EXPR_NOT:
        result = resolve_expression(expr_loc, cont_type, e->ref);
        check_integer_type(expr_loc, cont_type, result.type);
        result.is_variable = FALSE;
        break;
    case EXPR_POS:
    case EXPR_NEG:
        result = resolve_expression(expr_loc, cont_type, e->ref);
        check_arithmetic_type(expr_loc, cont_type, result.type);
        result.is_variable = FALSE;
        break;
    case EXPR_ADDRESSOF:
        result = resolve_expression(expr_loc, cont_type, e->ref);
        if (!result.is_variable)
            error_loc_info(&expr_loc->v->loc_info, "address-of operator applied to non-variable type in expression%s%s\n",
                           expr_loc->attr ? " for attribute " : "",
                           expr_loc->attr ? expr_loc->attr : "");
            result.is_variable = FALSE;
        result.is_temporary = TRUE;
        result.type = make_type(RPC_FC_RP, result.type);
        break;
    case EXPR_PPTR:
        result = resolve_expression(expr_loc, cont_type, e->ref);
        if (result.type && is_ptr(result.type))
            result.type = result.type->ref;
        else
            error_loc_info(&expr_loc->v->loc_info, "dereference operator applied to non-pointer type in expression%s%s\n",
                           expr_loc->attr ? " for attribute " : "",
                           expr_loc->attr ? expr_loc->attr : "");
        break;
    case EXPR_CAST:
        result = resolve_expression(expr_loc, cont_type, e->ref);
        result.type = e->u.tref;
        break;
    case EXPR_SIZEOF:
        result.is_variable = FALSE;
        result.is_temporary = FALSE;
        result.type = find_type("int", 0);
        break;
    case EXPR_SHL:
    case EXPR_SHR:
    case EXPR_MOD:
    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_AND:
    case EXPR_OR:
    case EXPR_XOR:
    {
        struct expression_type result_right;
        result = resolve_expression(expr_loc, cont_type, e->ref);
        result.is_variable = FALSE;
        result_right = resolve_expression(expr_loc, cont_type, e->u.ext);
        /* FIXME: these checks aren't strict enough for some of the operators */
        check_scalar_type(expr_loc, cont_type, result.type);
        check_scalar_type(expr_loc, cont_type, result_right.type);
        break;
    }
    case EXPR_LOGOR:
    case EXPR_LOGAND:
    case EXPR_EQUALITY:
    case EXPR_INEQUALITY:
    case EXPR_GTR:
    case EXPR_LESS:
    case EXPR_GTREQL:
    case EXPR_LESSEQL:
    {
        struct expression_type result_left, result_right;
        result_left = resolve_expression(expr_loc, cont_type, e->ref);
        result_right = resolve_expression(expr_loc, cont_type, e->u.ext);
        check_scalar_type(expr_loc, cont_type, result_left.type);
        check_scalar_type(expr_loc, cont_type, result_right.type);
        result.is_variable = FALSE;
        result.is_temporary = FALSE;
        result.type = find_type("int", 0);
        break;
    }
    case EXPR_MEMBER:
        result = resolve_expression(expr_loc, cont_type, e->ref);
        if (result.type && (is_struct(result.type->type) || is_union(result.type->type) || result.type->type == RPC_FC_ENUM16 || result.type->type == RPC_FC_ENUM32))
            result = resolve_expression(expr_loc, result.type, e->u.ext);
        else
            error_loc_info(&expr_loc->v->loc_info, "'.' or '->' operator applied to a type that isn't a structure, union or enumeration in expression%s%s\n",
                           expr_loc->attr ? " for attribute " : "",
                           expr_loc->attr ? expr_loc->attr : "");
        break;
    case EXPR_COND:
    {
        struct expression_type result_first, result_second, result_third;
        result_first = resolve_expression(expr_loc, cont_type, e->ref);
        check_scalar_type(expr_loc, cont_type, result_first.type);
        result_second = resolve_expression(expr_loc, cont_type, e->u.ext);
        result_third = resolve_expression(expr_loc, cont_type, e->ext2);
        /* FIXME: determine the correct return type */
        result = result_second;
        result.is_variable = FALSE;
        break;
    }
    case EXPR_ARRAY:
        result = resolve_expression(expr_loc, cont_type, e->ref);
        if (result.type && is_array(result.type))
        {
            struct expression_type index_result;
            result.type = result.type->ref;
            index_result = resolve_expression(expr_loc, cont_type /* FIXME */, e->u.ext);
            if (!index_result.type || !is_integer_type(index_result.type))
                error_loc_info(&expr_loc->v->loc_info, "array subscript not of integral type in expression%s%s\n",
                               expr_loc->attr ? " for attribute " : "",
                               expr_loc->attr ? expr_loc->attr : "");
        }
        else
            error_loc_info(&expr_loc->v->loc_info, "array subscript operator applied to non-array type in expression%s%s\n",
                           expr_loc->attr ? " for attribute " : "",
                           expr_loc->attr ? expr_loc->attr : "");
        break;
    }
    return result;
}

const type_t *expr_resolve_type(const struct expr_loc *expr_loc, const type_t *cont_type, const expr_t *expr)
{
    struct expression_type expr_type;
    expr_type = resolve_expression(expr_loc, cont_type, expr);
    return expr_type.type;
}

void write_expr(FILE *h, const expr_t *e, int brackets,
                int toplevel, const char *toplevel_prefix,
                const type_t *cont_type, const char *local_var_prefix)
{
    switch (e->type)
    {
    case EXPR_VOID:
        break;
    case EXPR_NUM:
        fprintf(h, "%lu", e->u.lval);
        break;
    case EXPR_HEXNUM:
        fprintf(h, "0x%lx", e->u.lval);
        break;
    case EXPR_DOUBLE:
        fprintf(h, "%#.15g", e->u.dval);
        break;
    case EXPR_TRUEFALSE:
        if (e->u.lval == 0)
            fprintf(h, "FALSE");
        else
            fprintf(h, "TRUE");
        break;
    case EXPR_IDENTIFIER:
        if (toplevel && toplevel_prefix && cont_type)
        {
            int found_in_cont_type;
            find_identifier(e->u.sval, cont_type, &found_in_cont_type);
            if (found_in_cont_type)
            {
                fprintf(h, "%s%s", toplevel_prefix, e->u.sval);
                break;
            }
        }
        fprintf(h, "%s%s", local_var_prefix, e->u.sval);
        break;
    case EXPR_STRLIT:
        fprintf(h, "\"%s\"", e->u.sval);
        break;
    case EXPR_WSTRLIT:
        fprintf(h, "L\"%s\"", e->u.sval);
        break;
    case EXPR_LOGNOT:
        fprintf(h, "!");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        break;
    case EXPR_NOT:
        fprintf(h, "~");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        break;
    case EXPR_POS:
        fprintf(h, "+");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        break;
    case EXPR_NEG:
        fprintf(h, "-");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        break;
    case EXPR_ADDRESSOF:
        fprintf(h, "&");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        break;
    case EXPR_PPTR:
        fprintf(h, "*");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        break;
    case EXPR_CAST:
        fprintf(h, "(");
        write_type_decl(h, e->u.tref, NULL);
        fprintf(h, ")");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        break;
    case EXPR_SIZEOF:
        fprintf(h, "sizeof(");
        write_type_decl(h, e->u.tref, NULL);
        fprintf(h, ")");
        break;
    case EXPR_SHL:
    case EXPR_SHR:
    case EXPR_MOD:
    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_AND:
    case EXPR_OR:
    case EXPR_LOGOR:
    case EXPR_LOGAND:
    case EXPR_XOR:
    case EXPR_EQUALITY:
    case EXPR_INEQUALITY:
    case EXPR_GTR:
    case EXPR_LESS:
    case EXPR_GTREQL:
    case EXPR_LESSEQL:
        if (brackets) fprintf(h, "(");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        switch (e->type)
        {
        case EXPR_SHL:          fprintf(h, " << "); break;
        case EXPR_SHR:          fprintf(h, " >> "); break;
        case EXPR_MOD:          fprintf(h, " %% "); break;
        case EXPR_MUL:          fprintf(h, " * "); break;
        case EXPR_DIV:          fprintf(h, " / "); break;
        case EXPR_ADD:          fprintf(h, " + "); break;
        case EXPR_SUB:          fprintf(h, " - "); break;
        case EXPR_AND:          fprintf(h, " & "); break;
        case EXPR_OR:           fprintf(h, " | "); break;
        case EXPR_LOGOR:        fprintf(h, " || "); break;
        case EXPR_LOGAND:       fprintf(h, " && "); break;
        case EXPR_XOR:          fprintf(h, " ^ "); break;
        case EXPR_EQUALITY:     fprintf(h, " == "); break;
        case EXPR_INEQUALITY:   fprintf(h, " != "); break;
        case EXPR_GTR:          fprintf(h, " > "); break;
        case EXPR_LESS:         fprintf(h, " < "); break;
        case EXPR_GTREQL:       fprintf(h, " >= "); break;
        case EXPR_LESSEQL:      fprintf(h, " <= "); break;
        default: break;
        }
        write_expr(h, e->u.ext, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        if (brackets) fprintf(h, ")");
        break;
    case EXPR_MEMBER:
        if (brackets) fprintf(h, "(");
        if (e->ref->type == EXPR_PPTR)
        {
            write_expr(h, e->ref->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
            fprintf(h, "->");
        }
        else
        {
            write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
            fprintf(h, ".");
        }
        write_expr(h, e->u.ext, 1, 0, toplevel_prefix, cont_type, "");
        if (brackets) fprintf(h, ")");
        break;
    case EXPR_COND:
        if (brackets) fprintf(h, "(");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        fprintf(h, " ? ");
        write_expr(h, e->u.ext, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        fprintf(h, " : ");
        write_expr(h, e->ext2, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        if (brackets) fprintf(h, ")");
        break;
    case EXPR_ARRAY:
        if (brackets) fprintf(h, "(");
        write_expr(h, e->ref, 1, toplevel, toplevel_prefix, cont_type, local_var_prefix);
        fprintf(h, "[");
        write_expr(h, e->u.ext, 1, 1, toplevel_prefix, cont_type, local_var_prefix);
        fprintf(h, "]");
        if (brackets) fprintf(h, ")");
        break;
    }
}

/* This is actually fairly involved to implement precisely, due to the
   effects attributes may have and things like that.  Right now this is
   only used for optimization, so just check for a very small set of
   criteria that guarantee the types are equivalent; assume every thing
   else is different.   */
static int compare_type(const type_t *a, const type_t *b)
{
    if (a == b
        || (a->name
            && b->name
            && strcmp(a->name, b->name) == 0))
        return 0;
    /* Ordering doesn't need to be implemented yet.  */
    return 1;
}

int compare_expr(const expr_t *a, const expr_t *b)
{
    int ret;

    if (a->type != b->type)
        return a->type - b->type;

    switch (a->type)
    {
        case EXPR_NUM:
        case EXPR_HEXNUM:
        case EXPR_TRUEFALSE:
            return a->u.lval - b->u.lval;
        case EXPR_DOUBLE:
            return a->u.dval - b->u.dval;
        case EXPR_IDENTIFIER:
        case EXPR_STRLIT:
        case EXPR_WSTRLIT:
            return strcmp(a->u.sval, b->u.sval);
        case EXPR_COND:
            ret = compare_expr(a->ref, b->ref);
            if (ret != 0)
                return ret;
            ret = compare_expr(a->u.ext, b->u.ext);
            if (ret != 0)
                return ret;
            return compare_expr(a->ext2, b->ext2);
        case EXPR_OR:
        case EXPR_AND:
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MOD:
        case EXPR_MUL:
        case EXPR_DIV:
        case EXPR_SHL:
        case EXPR_SHR:
        case EXPR_MEMBER:
        case EXPR_ARRAY:
        case EXPR_LOGOR:
        case EXPR_LOGAND:
        case EXPR_XOR:
        case EXPR_EQUALITY:
        case EXPR_INEQUALITY:
        case EXPR_GTR:
        case EXPR_LESS:
        case EXPR_GTREQL:
        case EXPR_LESSEQL:
            ret = compare_expr(a->ref, b->ref);
            if (ret != 0)
                return ret;
            return compare_expr(a->u.ext, b->u.ext);
        case EXPR_CAST:
            ret = compare_type(a->u.tref, b->u.tref);
            if (ret != 0)
                return ret;
            /* Fall through.  */
        case EXPR_NOT:
        case EXPR_NEG:
        case EXPR_PPTR:
        case EXPR_ADDRESSOF:
        case EXPR_LOGNOT:
        case EXPR_POS:
            return compare_expr(a->ref, b->ref);
        case EXPR_SIZEOF:
            return compare_type(a->u.tref, b->u.tref);
        case EXPR_VOID:
            return 0;
    }
    return -1;
}

/*
 * IDL Compiler
 *
 * Copyright 2002 Ove Kaaven
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <string.h>
#include <ctype.h>

#include <Windows.h>
#include "widl.h"
#include "utils.h"
#include "parser.h"
#include "header.h"
#include "expr.h"

typedef struct _user_type_t generic_handle_t;

static int indentation = 0;
user_type_list_t user_type_list = LIST_INIT(user_type_list);
static context_handle_list_t context_handle_list = LIST_INIT(context_handle_list);
static struct list generic_handle_list = LIST_INIT(generic_handle_list);

static void indent(FILE *h, int delta)
{
  int c;
  if (delta < 0) indentation += delta;
  for (c=0; c<indentation; c++) fprintf(h, "    ");
  if (delta > 0) indentation += delta;
}

int is_ptrchain_attr(const var_t *var, enum attr_type t)
{
    if (is_attr(var->attrs, t))
        return 1;
    else
    {
        type_t *type = var->type;
        for (;;)
        {
            if (is_attr(type->attrs, t))
                return 1;
            else if (type->kind == TKIND_ALIAS)
                type = type->orig;
            else if (is_ptr(type))
                type = type->ref;
            else return 0;
        }
    }
}

int is_aliaschain_attr(const type_t *type, enum attr_type attr)
{
    const type_t *t = type;
    for (;;)
    {
        if (is_attr(t->attrs, attr))
            return 1;
        else if (t->kind == TKIND_ALIAS)
            t = t->orig;
        else return 0;
    }
}

int is_attr(const attr_list_t *list, enum attr_type t)
{
    const attr_t *attr;
    if (list) LIST_FOR_EACH_ENTRY( attr, list, const attr_t, entry )
        if (attr->type == t) return 1;
    return 0;
}

void *get_attrp(const attr_list_t *list, enum attr_type t)
{
    const attr_t *attr;
    if (list) LIST_FOR_EACH_ENTRY( attr, list, const attr_t, entry )
        if (attr->type == t) return attr->u.pval;
    return NULL;
}

unsigned long get_attrv(const attr_list_t *list, enum attr_type t)
{
    const attr_t *attr;
    if (list) LIST_FOR_EACH_ENTRY( attr, list, const attr_t, entry )
        if (attr->type == t) return attr->u.ival;
    return 0;
}

int is_void(const type_t *t)
{
  if (!t->type && !t->ref) return 1;
  return 0;
}

int is_conformant_array(const type_t *t)
{
    return t->type == RPC_FC_CARRAY
        || t->type == RPC_FC_CVARRAY
        || (t->type == RPC_FC_BOGUS_ARRAY && t->size_is);
}

void write_guid(FILE *f, const char *guid_prefix, const char *name, const UUID *uuid)
{
  if (!uuid) return;
  fprintf(f, "DEFINE_GUID(%s_%s, 0x%08x, 0x%04x, 0x%04x, 0x%02x,0x%02x, 0x%02x,"
        "0x%02x,0x%02x,0x%02x,0x%02x,0x%02x);\n",
        guid_prefix, name, uuid->Data1, uuid->Data2, uuid->Data3, uuid->Data4[0],
        uuid->Data4[1], uuid->Data4[2], uuid->Data4[3], uuid->Data4[4], uuid->Data4[5],
        uuid->Data4[6], uuid->Data4[7]);
}

const char *get_name(const var_t *v)
{
    static char buffer[256];

    if (is_attr( v->attrs, ATTR_PROPGET ))
        strcpy( buffer, "get_" );
    else if (is_attr( v->attrs, ATTR_PROPPUT ))
        strcpy( buffer, "put_" );
    else if (is_attr( v->attrs, ATTR_PROPPUTREF ))
        strcpy( buffer, "putref_" );
    else
        buffer[0] = 0;
    strcat( buffer, v->name );
    return buffer;
}

static void write_field(FILE *h, var_t *v)
{
  if (!v) return;
  if (v->type) {
    indent(h, 0);
    write_type_def_or_decl(h, v->type, TRUE, "%s", v->name);
    fprintf(h, ";\n");
  }
}

static void write_fields(FILE *h, var_list_t *fields)
{
    var_t *v;
    if (!fields) return;
    LIST_FOR_EACH_ENTRY( v, fields, var_t, entry ) write_field(h, v);
}

static void write_enums(FILE *h, var_list_t *enums)
{
  var_t *v;
  if (!enums) return;
  LIST_FOR_EACH_ENTRY( v, enums, var_t, entry )
  {
    if (v->name) {
      indent(h, 0);
      fprintf(h, "%s", get_name(v));
      if (v->eval) {
        fprintf(h, " = ");
        write_expr(h, v->eval, 0, 1, NULL, NULL, "");
      }
    }
    if (list_next( enums, &v->entry )) fprintf(h, ",\n");
  }
  fprintf(h, "\n");
}

int needs_space_after(type_t *t)
{
  return (t->kind == TKIND_ALIAS
          || (!is_ptr(t) && (!is_conformant_array(t) || t->declarray)));
}

void write_type_left(FILE *h, type_t *t, int declonly)
{
  if (!h) return;

  if (is_attr(t->attrs, ATTR_CONST) &&
      (t->kind == TKIND_ALIAS || t->declarray || !is_ptr(t)))
    fprintf(h, "const ");

  if (t->kind == TKIND_ALIAS) fprintf(h, "%s", t->name);
  else if (t->declarray) write_type_left(h, t->ref, declonly);
  else {
    if (t->sign > 0) fprintf(h, "signed ");
    else if (t->sign < 0) fprintf(h, "unsigned ");
    switch (t->type) {
      case RPC_FC_ENUM16:
      case RPC_FC_ENUM32:
        if (!declonly && t->defined && !t->written && !t->ignore) {
          if (t->name) fprintf(h, "enum %s {\n", t->name);
          else fprintf(h, "enum {\n");
          t->written = TRUE;
          indentation++;
          write_enums(h, t->fields_or_args);
          indent(h, -1);
          fprintf(h, "}");
        }
        else fprintf(h, "enum %s", t->name ? t->name : "");
        break;
      case RPC_FC_STRUCT:
      case RPC_FC_CVSTRUCT:
      case RPC_FC_CPSTRUCT:
      case RPC_FC_CSTRUCT:
      case RPC_FC_PSTRUCT:
      case RPC_FC_BOGUS_STRUCT:
      case RPC_FC_ENCAPSULATED_UNION:
        if (!declonly && t->defined && !t->written && !t->ignore) {
          if (t->name) fprintf(h, "struct %s {\n", t->name);
          else fprintf(h, "struct {\n");
          t->written = TRUE;
          indentation++;
          write_fields(h, t->fields_or_args);
          indent(h, -1);
          fprintf(h, "}");
        }
        else fprintf(h, "struct %s", t->name ? t->name : "");
        break;
      case RPC_FC_NON_ENCAPSULATED_UNION:
        if (!declonly && t->defined && !t->written && !t->ignore) {
          if (t->name) fprintf(h, "union %s {\n", t->name);
          else fprintf(h, "union {\n");
          t->written = TRUE;
          indentation++;
          write_fields(h, t->fields_or_args);
          indent(h, -1);
          fprintf(h, "}");
        }
        else fprintf(h, "union %s", t->name ? t->name : "");
        break;
      case RPC_FC_RP:
      case RPC_FC_UP:
      case RPC_FC_FP:
      case RPC_FC_OP:
      case RPC_FC_CARRAY:
      case RPC_FC_CVARRAY:
      case RPC_FC_BOGUS_ARRAY:
        write_type_left(h, t->ref, declonly);
        fprintf(h, "%s*", needs_space_after(t->ref) ? " " : "");
        if (is_ptr(t) && is_attr(t->attrs, ATTR_CONST)) fprintf(h, "const ");
        break;
      default:
        fprintf(h, "%s", t->name);
    }
  }
}

void write_type_right(FILE *h, type_t *t, int is_field)
{
  if (!h) return;

  if (t->declarray) {
    if (is_conformant_array(t)) {
      fprintf(h, "[%s]", is_field ? "1" : "");
      t = t->ref;
    }
    for ( ; t->declarray; t = t->ref)
      fprintf(h, "[%lu]", t->dim);
  }
}

void write_type_v(FILE *h, type_t *t, int is_field, int declonly,
                  const char *fmt, va_list args)
{
  type_t *pt;
  int ptr_level = 0;

  if (!h) return;

  for (pt = t; is_ptr(pt); pt = pt->ref, ptr_level++)
    ;

  if (pt->type == RPC_FC_FUNCTION) {
    int i;
    const char *callconv = get_attrp(pt->attrs, ATTR_CALLCONV);
    if (!callconv) callconv = "";
    if (is_attr(pt->attrs, ATTR_INLINE)) fprintf(h, "inline ");
    write_type_left(h, pt->ref, declonly);
    fputc(' ', h);
    if (ptr_level) fputc('(', h);
    fprintf(h, "%s ", callconv);
    for (i = 0; i < ptr_level; i++)
      fputc('*', h);
  } else
    write_type_left(h, t, declonly);
  if (fmt) {
    if (needs_space_after(t))
      fputc(' ', h);
    vfprintf(h, fmt, args);
  }
  if (pt->type == RPC_FC_FUNCTION) {
    if (ptr_level) fputc(')', h);
    fputc('(', h);
    write_args(h, pt->fields_or_args, NULL, 0, FALSE);
    fputc(')', h);
  } else
    write_type_right(h, t, is_field);
}

void write_type_def_or_decl(FILE *f, type_t *t, int field, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  write_type_v(f, t, field, FALSE, fmt, args);
  va_end(args);
}

void write_type_decl(FILE *f, type_t *t, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  write_type_v(f, t, FALSE, TRUE, fmt, args);
  va_end(args);
}

void write_type_decl_left(FILE *f, type_t *t)
{
  write_type_left(f, t, TRUE);
}

static int user_type_registered(const char *name)
{
  user_type_t *ut;
  LIST_FOR_EACH_ENTRY(ut, &user_type_list, user_type_t, entry)
    if (!strcmp(name, ut->name))
      return 1;
  return 0;
}

static int context_handle_registered(const char *name)
{
  context_handle_t *ch;
  LIST_FOR_EACH_ENTRY(ch, &context_handle_list, context_handle_t, entry)
    if (!strcmp(name, ch->name))
      return 1;
  return 0;
}

static int generic_handle_registered(const char *name)
{
  generic_handle_t *gh;
  LIST_FOR_EACH_ENTRY(gh, &generic_handle_list, generic_handle_t, entry)
    if (!strcmp(name, gh->name))
      return 1;
  return 0;
}

/* check for types which require additional prototypes to be generated in the
 * header */
void check_for_additional_prototype_types(const var_list_t *list)
{
  const var_t *v;

  if (!list) return;
  LIST_FOR_EACH_ENTRY( v, list, const var_t, entry )
  {
    type_t *type;
    for (type = v->type; type; type = type->kind == TKIND_ALIAS ? type->orig : type->ref) {
      const char *name = type->name;
      if (type->user_types_registered) continue;
      type->user_types_registered = 1;
      if (is_attr(type->attrs, ATTR_CONTEXTHANDLE)) {
        if (!context_handle_registered(name))
        {
          context_handle_t *ch = xmalloc(sizeof(*ch));
          ch->name = xstrdup(name);
          list_add_tail(&context_handle_list, &ch->entry);
        }
        /* don't carry on parsing fields within this type */
        break;
      }
      if (type->type != RPC_FC_BIND_PRIMITIVE && is_attr(type->attrs, ATTR_HANDLE)) {
        if (!generic_handle_registered(name))
        {
          generic_handle_t *gh = xmalloc(sizeof(*gh));
          gh->name = xstrdup(name);
          list_add_tail(&generic_handle_list, &gh->entry);
        }
        /* don't carry on parsing fields within this type */
        break;
      }
      if (is_attr(type->attrs, ATTR_WIREMARSHAL)) {
        if (!user_type_registered(name))
        {
          user_type_t *ut = xmalloc(sizeof *ut);
          ut->name = xstrdup(name);
          list_add_tail(&user_type_list, &ut->entry);
        }
        /* don't carry on parsing fields within this type as we are already
         * using a wire marshaled type */
        break;
      }
      else
      {
        check_for_additional_prototype_types(type->fields_or_args);
      }
    }
  }
}

void write_user_types(void)
{
  user_type_t *ut;
  LIST_FOR_EACH_ENTRY(ut, &user_type_list, user_type_t, entry)
  {
    const char *name = ut->name;
    fprintf(header, "ULONG           __RPC_USER %s_UserSize     (ULONG *, ULONG, %s *);\n", name, name);
    fprintf(header, "unsigned char * __RPC_USER %s_UserMarshal  (ULONG *, unsigned char *, %s *);\n", name, name);
    fprintf(header, "unsigned char * __RPC_USER %s_UserUnmarshal(ULONG *, unsigned char *, %s *);\n", name, name);
    fprintf(header, "void            __RPC_USER %s_UserFree     (ULONG *, %s *);\n", name, name);
  }
}

void write_context_handle_rundowns(void)
{
  context_handle_t *ch;
  LIST_FOR_EACH_ENTRY(ch, &context_handle_list, context_handle_t, entry)
  {
    const char *name = ch->name;
    fprintf(header, "void __RPC_USER %s_rundown(%s);\n", name, name);
  }
}

void write_generic_handle_routines(void)
{
  generic_handle_t *gh;
  LIST_FOR_EACH_ENTRY(gh, &generic_handle_list, generic_handle_t, entry)
  {
    const char *name = gh->name;
    fprintf(header, "handle_t __RPC_USER %s_bind(%s);\n", name, name);
    fprintf(header, "void __RPC_USER %s_unbind(%s, handle_t);\n", name, name);
  }
}

void write_typedef(type_t *type)
{
  fprintf(header, "typedef ");
  write_type_def_or_decl(header, type->orig, FALSE, "%s", type->name);
  fprintf(header, ";\n");
}

int is_const_decl(const var_t *var)
{
  const type_t *t;
  /* strangely, MIDL accepts a const attribute on any pointer in the
  * declaration to mean that data isn't being instantiated. this appears
  * to be a bug, but there is no benefit to being incompatible with MIDL,
  * so we'll do the same thing */
  for (t = var->type; ; )
  {
    if (is_attr(t->attrs, ATTR_CONST))
      return TRUE;
    else if (is_ptr(t))
      t = t->ref;
    else break;
  }
  return FALSE;
}

void write_declaration(const var_t *v, int is_in_interface)
{
  if (is_const_decl(v) && v->eval)
  {
    fprintf(header, "#define %s (", v->name);
    write_expr(header, v->eval, 0, 1, NULL, NULL, "");
    fprintf(header, ")\n\n");
  }
  else if (v->type->type != RPC_FC_FUNCTION || !is_in_interface)
  {
    switch (v->stgclass)
    {
      case STG_NONE:
      case STG_REGISTER: /* ignored */
        break;
      case STG_STATIC:
        fprintf(header, "static ");
        break;
      case STG_EXTERN:
        fprintf(header, "extern ");
        break;
    }
    write_type_def_or_decl(header, v->type, FALSE, "%s", v->name);
    fprintf(header, ";\n\n");
  }
}

void write_library(const typelib_t *typelib)
{
  const UUID *uuid = get_attrp(typelib->attrs, ATTR_UUID);
  fprintf(header, "\n");
  write_guid(header, "LIBID", typelib->name, uuid);
  fprintf(header, "\n");
}


const var_t* get_explicit_handle_var(const func_t* func)
{
    const var_t* var;

    if (!func->args)
        return NULL;

    LIST_FOR_EACH_ENTRY( var, func->args, const var_t, entry )
        if (var->type->type == RPC_FC_BIND_PRIMITIVE)
            return var;

    return NULL;
}

const type_t* get_explicit_generic_handle_type(const var_t* var)
{
    const type_t *t;
    for (t = var->type; is_ptr(t); t = t->ref)
        if (t->type != RPC_FC_BIND_PRIMITIVE && is_attr(t->attrs, ATTR_HANDLE))
            return t;
    return NULL;
}

const var_t* get_explicit_generic_handle_var(const func_t* func)
{
    const var_t* var;

    if (!func->args)
        return NULL;

    LIST_FOR_EACH_ENTRY( var, func->args, const var_t, entry )
        if (get_explicit_generic_handle_type(var))
            return var;

    return NULL;
}

const var_t* get_context_handle_var(const func_t* func)
{
    const var_t* var;

    if (!func->args)
        return NULL;

    LIST_FOR_EACH_ENTRY( var, func->args, const var_t, entry )
        if (is_attr(var->attrs, ATTR_IN) && is_context_handle(var->type))
            return var;

    return NULL;
}

int has_out_arg_or_return(const func_t *func)
{
    const var_t *var;

    if (!is_void(get_func_return_type(func)))
        return 1;

    if (!func->args)
        return 0;

    LIST_FOR_EACH_ENTRY( var, func->args, const var_t, entry )
        if (is_attr(var->attrs, ATTR_OUT))
            return 1;

    return 0;
}


/********** INTERFACES **********/

int is_object(const attr_list_t *list)
{
    const attr_t *attr;
    if (list) LIST_FOR_EACH_ENTRY( attr, list, const attr_t, entry )
        if (attr->type == ATTR_OBJECT || attr->type == ATTR_ODL) return 1;
    return 0;
}

int is_local(const attr_list_t *a)
{
  return is_attr(a, ATTR_LOCAL);
}

const var_t *is_callas(const attr_list_t *a)
{
  return get_attrp(a, ATTR_CALLAS);
}

static void write_method_macro(FILE *header, const type_t *iface, const char *name)
{
  const func_t *cur;

  if (iface->ref) write_method_macro(header, iface->ref, name);

  if (!iface->funcs) return;

  fprintf(header, "/*** %s methods ***/\n", iface->name);
  LIST_FOR_EACH_ENTRY( cur, iface->funcs, const func_t, entry )
  {
    var_t *def = cur->def;
    if (!is_callas(def->attrs)) {
      const var_t *arg;

      fprintf(header, "#define %s_%s(This", name, get_name(def));
      if (cur->args)
          LIST_FOR_EACH_ENTRY( arg, cur->args, const var_t, entry )
              fprintf(header, ",%s", arg->name);
      fprintf(header, ") ");

      fprintf(header, "(This)->lpVtbl->%s(This", get_name(def));
      if (cur->args)
          LIST_FOR_EACH_ENTRY( arg, cur->args, const var_t, entry )
              fprintf(header, ",%s", arg->name);
      fprintf(header, ")\n");
    }
  }
}

void write_args(FILE *h, const var_list_t *args, const char *name, int method, int do_indent)
{
  const var_t *arg;
  int count = 0;

  if (do_indent)
  {
      indentation++;
      indent(h, 0);
  }
  if (method == 1) {
    fprintf(h, "%s* This", name);
    count++;
  }
  if (args) LIST_FOR_EACH_ENTRY( arg, args, const var_t, entry ) {
    if (count) {
        if (do_indent)
        {
            fprintf(h, ",\n");
            indent(h, 0);
        }
        else fprintf(h, ",");
    }
    write_type_decl(h, arg->type, "%s", arg->name);
    count++;
  }
  if (do_indent) indentation--;
}

static void write_cpp_method_def(FILE *header, const type_t *iface)
{
  const func_t *cur;

  if (!iface->funcs) return;

  LIST_FOR_EACH_ENTRY( cur, iface->funcs, const func_t, entry )
  {
    var_t *def = cur->def;
    if (!is_callas(def->attrs)) {
      const char *callconv = get_attrp(def->type->attrs, ATTR_CALLCONV);
      if (!callconv) callconv = "";
      indent(header, 0);
      fprintf(header, "virtual ");
      write_type_decl_left(header, get_func_return_type(cur));
      fprintf(header, " %s %s(\n", callconv, get_name(def));
      write_args(header, cur->args, iface->name, 2, TRUE);
      fprintf(header, ") = 0;\n");
      fprintf(header, "\n");
    }
  }
}

static void do_write_c_method_def(FILE *header, const type_t *iface, const char *name)
{
  const func_t *cur;

  if (iface->ref) do_write_c_method_def(header, iface->ref, name);

  if (!iface->funcs) return;
  indent(header, 0);
  fprintf(header, "/*** %s methods ***/\n", iface->name);
  LIST_FOR_EACH_ENTRY( cur, iface->funcs, const func_t, entry )
  {
    const var_t *def = cur->def;
    if (!is_callas(def->attrs)) {
      const char *callconv = get_attrp(def->type->attrs, ATTR_CALLCONV);
      if (!callconv) callconv = "";
      indent(header, 0);
      write_type_decl_left(header, get_func_return_type(cur));
      fprintf(header, " (%s *%s)(\n", callconv, get_name(def));
      write_args(header, cur->args, name, 1, TRUE);
      fprintf(header, ");\n");
      fprintf(header, "\n");
    }
  }
}

static void write_c_method_def(FILE *header, const type_t *iface)
{
  do_write_c_method_def(header, iface, iface->name);
}

static void write_c_disp_method_def(FILE *header, const type_t *iface)
{
  do_write_c_method_def(header, iface->ref, iface->name);
}

static void write_method_proto(FILE *header, const type_t *iface)
{
  const func_t *cur;

  if (!iface->funcs) return;
  LIST_FOR_EACH_ENTRY( cur, iface->funcs, const func_t, entry )
  {
    const var_t *def = cur->def;

    if (!is_local(def->attrs)) {
      const char *callconv = get_attrp(def->type->attrs, ATTR_CALLCONV);
      if (!callconv) callconv = "";
      /* proxy prototype */
      write_type_decl_left(header, get_func_return_type(cur));
      fprintf(header, " %s %s_%s_Proxy(\n", callconv, iface->name, get_name(def));
      write_args(header, cur->args, iface->name, 1, TRUE);
      fprintf(header, ");\n");
      /* stub prototype */
      fprintf(header, "void __RPC_STUB %s_%s_Stub(\n", iface->name, get_name(def));
      fprintf(header, "    IRpcStubBuffer* This,\n");
      fprintf(header, "    IRpcChannelBuffer* pRpcChannelBuffer,\n");
      fprintf(header, "    PRPC_MESSAGE pRpcMessage,\n");
      fprintf(header, "    DWORD* pdwStubPhase);\n");
    }
  }
}

void write_locals(FILE *fp, const type_t *iface, int body)
{
  static const char comment[]
    = "/* WIDL-generated stub.  You must provide an implementation for this.  */";
  const func_list_t *funcs = iface->funcs;
  const func_t *cur;

  if (!is_object(iface->attrs) || !funcs)
    return;

  LIST_FOR_EACH_ENTRY(cur, funcs, const func_t, entry) {
    const var_t *def = cur->def;
    const var_t *cas = is_callas(def->attrs);

    if (cas) {
      const func_t *m;
      LIST_FOR_EACH_ENTRY(m, iface->funcs, const func_t, entry)
        if (!strcmp(m->def->name, cas->name))
          break;
      if (&m->entry != iface->funcs) {
        const var_t *mdef = m->def;
        /* proxy prototype - use local prototype */
        write_type_decl_left(fp, get_func_return_type(m));
        fprintf(fp, " CALLBACK %s_%s_Proxy(\n", iface->name, get_name(mdef));
        write_args(fp, m->args, iface->name, 1, TRUE);
        fprintf(fp, ")");
        if (body) {
          type_t *rt = get_func_return_type(m);
          fprintf(fp, "\n{\n");
          fprintf(fp, "    %s\n", comment);
          if (rt->name && strcmp(rt->name, "HRESULT") == 0)
            fprintf(fp, "    return E_NOTIMPL;\n");
          else if (rt->type) {
            fprintf(fp, "    ");
            write_type_decl(fp, rt, "rv");
            fprintf(fp, ";\n");
            fprintf(fp, "    memset(&rv, 0, sizeof rv);\n");
            fprintf(fp, "    return rv;\n");
          }
          fprintf(fp, "}\n\n");
        }
        else
          fprintf(fp, ";\n");
        /* stub prototype - use remotable prototype */
        write_type_decl_left(fp, get_func_return_type(cur));
        fprintf(fp, " __RPC_STUB %s_%s_Stub(\n", iface->name, get_name(mdef));
        write_args(fp, cur->args, iface->name, 1, TRUE);
        fprintf(fp, ")");
        if (body)
          /* Remotable methods must all return HRESULTs.  */
          fprintf(fp, "\n{\n    %s\n    return E_NOTIMPL;\n}\n\n", comment);
        else
          fprintf(fp, ";\n");
      }
      else
        error_loc("invalid call_as attribute (%s -> %s)\n", def->name, cas->name);
    }
  }
}

static void write_function_proto(FILE *header, const type_t *iface, const func_t *fun, const char *prefix)
{
  var_t *def = fun->def;
  const char *callconv = get_attrp(def->type->attrs, ATTR_CALLCONV);

  /* FIXME: do we need to handle call_as? */
  write_type_decl_left(header, get_func_return_type(fun));
  fprintf(header, " ");
  if (callconv) fprintf(header, "%s ", callconv);
  fprintf(header, "%s%s(\n", prefix, get_name(def));
  if (fun->args)
    write_args(header, fun->args, iface->name, 0, TRUE);
  else
    fprintf(header, "    void");
  fprintf(header, ");\n\n");
}

static void write_function_protos(FILE *header, const type_t *iface)
{
  const func_t *cur;
  int prefixes_differ = strcmp(prefix_client, prefix_server);

  if (!iface->funcs) return;
  LIST_FOR_EACH_ENTRY( cur, iface->funcs, const func_t, entry )
  {
    if (prefixes_differ) {
      fprintf(header, "/* client prototype */\n");
      write_function_proto(header, iface, cur, prefix_client);
      fprintf(header, "/* server prototype */\n");
    }
    write_function_proto(header, iface, cur, prefix_server);
  }
}

void write_forward(type_t *iface)
{
  /* C/C++ forwards should only be written for object interfaces, so if we
   * have a full definition we only write one if we find [object] among the
   * attributes - however, if we don't have a full definition at this point
   * (i.e. this is an IDL forward), then we also assume that it is an object
   * interface, since non-object interfaces shouldn't need forwards */
  if ((!iface->defined || is_object(iface->attrs) || is_attr(iface->attrs, ATTR_DISPINTERFACE))
        && !iface->written) {
    fprintf(header, "#ifndef __%s_FWD_DEFINED__\n", iface->name);
    fprintf(header, "#define __%s_FWD_DEFINED__\n", iface->name);
    fprintf(header, "typedef interface %s %s;\n", iface->name, iface->name);
    fprintf(header, "#endif\n\n" );
    iface->written = TRUE;
  }
}

static void write_iface_guid(FILE *header, const type_t *iface)
{
  const UUID *uuid = get_attrp(iface->attrs, ATTR_UUID);
  write_guid(header, "IID", iface->name, uuid);
} 

static void write_dispiface_guid(FILE *header, const type_t *iface)
{
  const UUID *uuid = get_attrp(iface->attrs, ATTR_UUID);
  write_guid(header, "DIID", iface->name, uuid);
}

static void write_coclass_guid(FILE *header, const type_t *cocl)
{
  const UUID *uuid = get_attrp(cocl->attrs, ATTR_UUID);
  write_guid(header, "CLSID", cocl->name, uuid);
}

static void write_com_interface_start(FILE *header, const type_t *iface)
{
  int dispinterface = is_attr(iface->attrs, ATTR_DISPINTERFACE);
  fprintf(header, "/*****************************************************************************\n");
  fprintf(header, " * %s %sinterface\n", iface->name, dispinterface ? "disp" : "");
  fprintf(header, " */\n");
  fprintf(header,"#ifndef __%s_%sINTERFACE_DEFINED__\n", iface->name, dispinterface ? "DISP" : "");
  fprintf(header,"#define __%s_%sINTERFACE_DEFINED__\n\n", iface->name, dispinterface ? "DISP" : "");
}

static void write_com_interface_end(FILE *header, type_t *iface)
{
  int dispinterface = is_attr(iface->attrs, ATTR_DISPINTERFACE);
  if (dispinterface)
    write_dispiface_guid(header, iface);
  else
    write_iface_guid(header, iface);
  /* C++ interface */
  fprintf(header, "#if defined(__cplusplus) && !defined(CINTERFACE)\n");
  if (iface->ref)
  {
    fprintf(header, "interface %s : public %s\n", iface->name, iface->ref->name);
    fprintf(header, "{\n");
  }
  else
  {
    fprintf(header, "interface %s\n", iface->name);
    fprintf(header, "{\n");
    fprintf(header, "    BEGIN_INTERFACE\n");
    fprintf(header, "\n");
  }
  /* dispinterfaces don't have real functions, so don't write C++ functions for
   * them */
  if (!dispinterface)
  {
    indentation++;
    write_cpp_method_def(header, iface);
    indentation--;
  }
  if (!iface->ref)
    fprintf(header, "    END_INTERFACE\n");
  fprintf(header, "};\n");
  fprintf(header, "#else\n");
  /* C interface */
  fprintf(header, "typedef struct %sVtbl {\n", iface->name);
  indentation++;
  fprintf(header, "    BEGIN_INTERFACE\n");
  fprintf(header, "\n");
  if (dispinterface)
    write_c_disp_method_def(header, iface);
  else
    write_c_method_def(header, iface);
  indentation--;
  fprintf(header, "    END_INTERFACE\n");
  fprintf(header, "} %sVtbl;\n", iface->name);
  fprintf(header, "interface %s {\n", iface->name);
  fprintf(header, "    CONST_VTBL %sVtbl* lpVtbl;\n", iface->name);
  fprintf(header, "};\n");
  fprintf(header, "\n");
  fprintf(header, "#ifdef COBJMACROS\n");
  /* dispinterfaces don't have real functions, so don't write macros for them,
   * only for the interface this interface inherits from, i.e. IDispatch */
  write_method_macro(header, dispinterface ? iface->ref : iface, iface->name);
  fprintf(header, "#endif\n");
  fprintf(header, "\n");
  fprintf(header, "#endif\n");
  fprintf(header, "\n");
  /* dispinterfaces don't have real functions, so don't write prototypes for
   * them */
  if (!dispinterface)
  {
    write_method_proto(header, iface);
    write_locals(header, iface, FALSE);
    fprintf(header, "\n");
  }
  fprintf(header,"#endif  /* __%s_%sINTERFACE_DEFINED__ */\n\n", iface->name, dispinterface ? "DISP" : "");
}

static void write_rpc_interface_start(FILE *header, const type_t *iface)
{
  unsigned int ver = get_attrv(iface->attrs, ATTR_VERSION);
  const char *var = get_attrp(iface->attrs, ATTR_IMPLICIT_HANDLE);
  static int allocate_written = 0;

  if (!allocate_written)
  {
    allocate_written = 1;
    fprintf(header, "void * __RPC_USER MIDL_user_allocate(size_t);\n");
    fprintf(header, "void __RPC_USER MIDL_user_free(void *);\n\n");
  }

  fprintf(header, "/*****************************************************************************\n");
  fprintf(header, " * %s interface (v%d.%d)\n", iface->name, MAJORVERSION(ver), MINORVERSION(ver));
  fprintf(header, " */\n");
  fprintf(header,"#ifndef __%s_INTERFACE_DEFINED__\n", iface->name);
  fprintf(header,"#define __%s_INTERFACE_DEFINED__\n\n", iface->name);
  if (var) fprintf(header, "extern handle_t %s;\n", var);
  if (old_names)
  {
      fprintf(header, "extern RPC_IF_HANDLE %s%s_ClientIfHandle;\n", prefix_client, iface->name);
      fprintf(header, "extern RPC_IF_HANDLE %s%s_ServerIfHandle;\n", prefix_server, iface->name);
  }
  else
  {
      fprintf(header, "extern RPC_IF_HANDLE %s%s_v%d_%d_c_ifspec;\n",
              prefix_client, iface->name, MAJORVERSION(ver), MINORVERSION(ver));
      fprintf(header, "extern RPC_IF_HANDLE %s%s_v%d_%d_s_ifspec;\n",
              prefix_server, iface->name, MAJORVERSION(ver), MINORVERSION(ver));
  }
}

static void write_rpc_interface_end(FILE *header, const type_t *iface)
{
  fprintf(header,"\n#endif  /* __%s_INTERFACE_DEFINED__ */\n\n", iface->name);
}

void write_interface(type_t *iface)
{
  if (is_attr(iface->attrs, ATTR_DISPINTERFACE) || is_object(iface->attrs))
  {
    write_com_interface_start(header, iface);
    write_com_interface_end(header, iface);
  }
  else
  {
    write_rpc_interface_start(header, iface);
    write_function_protos(header, iface);
    write_rpc_interface_end(header, iface);
  }
}

void write_coclass(type_t *cocl)
{
  fprintf(header, "/*****************************************************************************\n");
  fprintf(header, " * %s coclass\n", cocl->name);
  fprintf(header, " */\n\n");
  write_coclass_guid(header, cocl);
  fprintf(header, "\n");
}

void write_coclass_forward(type_t *cocl)
{
  fprintf(header, "#ifndef __%s_FWD_DEFINED__\n", cocl->name);
  fprintf(header, "#define __%s_FWD_DEFINED__\n", cocl->name);
  fprintf(header, "typedef struct %s %s;\n", cocl->name, cocl->name);
  fprintf(header, "#endif /* defined __%s_FWD_DEFINED__ */\n\n", cocl->name );
}

void write_import(const char *fname)
{
  char *hname, *p;

  hname = dup_basename(fname, ".idl");
  p = hname + strlen(hname) - 2;
  if (p <= hname || strcmp( p, ".h" )) strcat(hname, ".h");

  fprintf(header, "#include <%s>\n", hname);
  free(hname);
}

int last_ptr(const type_t *type)
{
	return is_ptr(type) && !is_declptr(type->ref);
}

int last_array(const type_t *type)
{
	return is_array(type) && !is_array(type->ref);
}

int is_string_type(const attr_list_t *attrs, const type_t *type)
{
	return ((is_attr(attrs, ATTR_STRING) || is_aliaschain_attr(type, ATTR_STRING))
		&& (last_ptr(type) || last_array(type)));
}

int is_context_handle(const type_t *type)
{
	const type_t *t;
	for (t = type; is_ptr(t); t = t->ref)
		if (is_attr(t->attrs, ATTR_CONTEXTHANDLE))
			return 1;
	return 0;
}

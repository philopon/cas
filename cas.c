#include <stdio.h>

#include <postgres.h>
#include <fmgr.h>
#include <math.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

typedef struct CAS {
  unsigned int first;
  unsigned int second;
  unsigned int check;
} CAS;


PG_FUNCTION_INFO_V1(cas_in);
PG_FUNCTION_INFO_V1(cas_out);

PG_FUNCTION_INFO_V1(cas_first);
PG_FUNCTION_INFO_V1(cas_second);
PG_FUNCTION_INFO_V1(cas_checkdigit);

PG_FUNCTION_INFO_V1(cas_cmp);
PG_FUNCTION_INFO_V1(cas_eq);
PG_FUNCTION_INFO_V1(cas_ne);
PG_FUNCTION_INFO_V1(cas_gt);
PG_FUNCTION_INFO_V1(cas_ge);
PG_FUNCTION_INFO_V1(cas_lt);
PG_FUNCTION_INFO_V1(cas_le);

Datum cas_in(PG_FUNCTION_ARGS);
Datum cas_out(PG_FUNCTION_ARGS);

Datum cas_first(PG_FUNCTION_ARGS);
Datum cas_second(PG_FUNCTION_ARGS);
Datum cas_checkdigit(PG_FUNCTION_ARGS);

static int cas_cmp_internal(CAS* a, CAS* b);
Datum cas_cmp(PG_FUNCTION_ARGS);
Datum cas_eq(PG_FUNCTION_ARGS);
Datum cas_ne(PG_FUNCTION_ARGS);
Datum cas_gt(PG_FUNCTION_ARGS);
Datum cas_ge(PG_FUNCTION_ARGS);
Datum cas_lt(PG_FUNCTION_ARGS);
Datum cas_le(PG_FUNCTION_ARGS);

Datum
cas_in(PG_FUNCTION_ARGS)
{
  char* str = PG_GETARG_CSTRING(0);
  unsigned int first, second, check;
  CAS* result;

  if (sscanf(str, "%7d-%02d-%1d", &first, &second, &check) != 3)
    ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                    errmsg("invalid input syntax for CAS: \"%s\"", str)));
  
  result = (CAS*) palloc(sizeof(CAS));
  result->first = first;
  result->second = second;
  result->check = check;
  PG_RETURN_POINTER(result);
}

Datum
cas_out(PG_FUNCTION_ARGS)
{
  CAS* cas = (CAS*) PG_GETARG_POINTER(0);
  char* result;
  unsigned int digit;

  // first(floor(log(f)) + 1)-second(2)-check(1)\0 : (floor(log10(first)) + 1) + 1 + 2 + 1 + 1 + 1
  digit = floor(log10(cas->first)) + 7; 
  result = palloc(digit);
  snprintf(result, digit, "%d-%02d-%d", cas->first, cas->second, cas->check);
  PG_RETURN_CSTRING(result);
}

Datum
cas_first(PG_FUNCTION_ARGS)
{
  CAS *cas;
  cas = (CAS*) PG_GETARG_POINTER(0);
  PG_RETURN_UINT32(cas->first);
}

Datum
cas_second(PG_FUNCTION_ARGS)
{
  CAS *cas;
  cas = (CAS*) PG_GETARG_POINTER(0);
  PG_RETURN_UINT32(cas->second);
}

Datum
cas_checkdigit(PG_FUNCTION_ARGS)
{
  CAS *cas;
  cas = (CAS*) PG_GETARG_POINTER(0);
  PG_RETURN_UINT32(cas->check);
}

static int
cas_cmp_internal(CAS* a, CAS* b)
{
  if(a->first != b->first) return (a->first > b->first)? 1: -1;
  if(a->second != b->second) return (a->second > b->second)? 1: -1;
  if(a->check != b->check) return (a->check > b->check)? 1: -1;
  return 0;
}

Datum
cas_cmp(PG_FUNCTION_ARGS)
{
  CAS *a, *b;
  a = (CAS*) PG_GETARG_POINTER(0);
  b = (CAS*) PG_GETARG_POINTER(1);

  PG_RETURN_INT32(cas_cmp_internal(a,b));
}

Datum
cas_eq(PG_FUNCTION_ARGS)
{
  CAS *a, *b;
  a = (CAS*) PG_GETARG_POINTER(0);
  b = (CAS*) PG_GETARG_POINTER(1);

  PG_RETURN_BOOL(cas_cmp_internal(a,b) == 0);
}

Datum
cas_ne(PG_FUNCTION_ARGS)
{
  CAS *a, *b;
  a = (CAS*) PG_GETARG_POINTER(0);
  b = (CAS*) PG_GETARG_POINTER(1);

  PG_RETURN_BOOL(cas_cmp_internal(a,b) != 0);
}

Datum
cas_gt(PG_FUNCTION_ARGS)
{
  CAS *a, *b;
  a = (CAS*) PG_GETARG_POINTER(0);
  b = (CAS*) PG_GETARG_POINTER(1);

  PG_RETURN_BOOL(cas_cmp_internal(a,b) > 0);
}

Datum
cas_ge(PG_FUNCTION_ARGS)
{
  CAS *a, *b;
  a = (CAS*) PG_GETARG_POINTER(0);
  b = (CAS*) PG_GETARG_POINTER(1);

  PG_RETURN_BOOL(cas_cmp_internal(a,b) >= 0);
}

Datum
cas_lt(PG_FUNCTION_ARGS)
{
  CAS *a, *b;
  a = (CAS*) PG_GETARG_POINTER(0);
  b = (CAS*) PG_GETARG_POINTER(1);

  PG_RETURN_BOOL(cas_cmp_internal(a,b) < 0);
}

Datum
cas_le(PG_FUNCTION_ARGS)
{
  CAS *a, *b;
  a = (CAS*) PG_GETARG_POINTER(0);
  b = (CAS*) PG_GETARG_POINTER(1);

  PG_RETURN_BOOL(cas_cmp_internal(a,b) <= 0);
}

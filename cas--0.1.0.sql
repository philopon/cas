CREATE FUNCTION cas_in(cstring)
       RETURNS cas
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_out(cas)
       RETURNS cstring
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_first(cas)
       RETURNS int
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_second(cas)
       RETURNS int
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_checkdigit(cas)
       RETURNS int
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_cmp(cas, cas)
       RETURNS integer
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_eq(cas, cas)
       RETURNS bool
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_ne(cas, cas)
       RETURNS bool
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_gt(cas, cas)
       RETURNS bool
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_ge(cas, cas)
       RETURNS bool
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_lt(cas, cas)
       RETURNS bool
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION cas_le(cas, cas)
       RETURNS bool
       AS 'MODULE_PATHNAME'
       LANGUAGE C IMMUTABLE STRICT;


CREATE TYPE CAS (
       INTERNALLENGTH = 12,
       INPUT = cas_in,
       OUTPUT = cas_out
);

CREATE OPERATOR = (
       PROCEDURE = cas_eq,
       LEFTARG = cas,
       RIGHTARG = cas,
       COMMUTATOR = =,
       NEGATOR = !=
);
       
CREATE OPERATOR != (
       PROCEDURE = cas_ne,
       LEFTARG = cas,
       RIGHTARG = cas,
       COMMUTATOR = !=,
       NEGATOR = =
);

CREATE OPERATOR > (
       PROCEDURE = cas_gt,
       LEFTARG = cas,
       RIGHTARG = cas,
       COMMUTATOR = <,
       NEGATOR = <=
);

CREATE OPERATOR >= (
       PROCEDURE = cas_ge,
       LEFTARG = cas,
       RIGHTARG = cas,
       COMMUTATOR = <=,
       NEGATOR = <
);

CREATE OPERATOR < (
       PROCEDURE = cas_lt,
       LEFTARG = cas,
       RIGHTARG = cas,
       COMMUTATOR = >,
       NEGATOR = >=
);

CREATE OPERATOR <= (
       PROCEDURE = cas_le,
       LEFTARG = cas,
       RIGHTARG = cas,
       COMMUTATOR = >=,
       NEGATOR = >
);

CREATE OPERATOR CLASS cas_ops
    DEFAULT FOR TYPE cas USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       cas_cmp(cas, cas);

MODULES = cas
EXTENSION = cas
DATA = cas--0.1.0.sql
REGRESS = type eq_ne digit ord

PG_CONFIG = /usr/local/bin/pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)


##############################
### VARIABLES
##############################

DEPS_DIR ?= ./deps

LIBBASE  := $(DEPS_DIR)/libbase
LIBMAYBE := $(DEPS_DIR)/libmaybe
LIBARRAY := $(DEPS_DIR)/libarray
LIBVEC   := $(DEPS_DIR)/libvec

CPPFLAGS += -I$(DEPS_DIR)

CFLAGS ?= -std=c11 -g \
          -Wall -Wextra -pedantic \
          -Wcomments -Wformat=2 -Wlogical-op -Wmissing-include-dirs \
          -Wnested-externs -Wold-style-definition -Wredundant-decls \
          -Wshadow -Wstrict-prototypes -Wunused-macros -Wvla -Wwrite-strings \
          -Wno-override-init -Wno-type-limits -Wno-unused-parameter

TPLRENDER ?= $(DEPS_DIR)/tplrender/tplrender


libbase_types  := char size
libmaybe_types := size
libarray_types := char
libvec_types   := char

char_type    := char
char_options := --typeclasses BOUNDED EQ ORD ENUM NUM \
                --extra num_type=signed char_funcs=true

size_type    := size_t
size_options := --typeclasses BOUNDED EQ ORD ENUM NUM \
                --extra num_type=unsigned

libbase_sources := $(foreach t,$(libbase_types),$(LIBBASE)/$t.c)
libbase_headers := $(libbase_sources:.c=.h)
libbase_objects := $(libbase_sources:.c=.o)

libmaybe_defs := $(foreach t,$(libmaybe_types),$(LIBMAYBE)/def/maybe-$t.h)

libarray_sources := $(foreach t,$(libarray_types),$(LIBARRAY)/array-$t.c)
libarray_headers := $(libarray_sources:.c=.h)
libarray_defs    := $(foreach t,$(libarray_types),$(LIBARRAY)/def/array-$t.h)
libarray_objects := $(libarray_sources:.c=.o)

libvec_sources := $(foreach t,$(libvec_types),$(LIBVEC)/vec-$t.c)
libvec_headers := $(libvec_sources:.c=.h)
libvec_defs    := $(foreach t,$(libvec_types),$(LIBVEC)/def/vec-$t.h)
libvec_objects := $(libvec_sources:.c=.o)

gen_objects := $(libbase_objects) \
               $(libarray_objects) \
               $(libvec_objects)

gen := $(libbase_sources) \
       $(libbase_headers) \
       $(libmaybe_defs) \
       $(libarray_sources) \
       $(libarray_headers) \
       $(libarray_defs) \
       $(libvec_sources) \
       $(libvec_headers) \
       $(libvec_defs) \
       $(gen_objects)

test_binaries := $(basename $(wildcard tests/*.c))

objects := string.o
mkdeps  := $(objects:.o=.dep.mk) $(gen_objects:.o=.dep.mk)



##############################
### BUILDING
##############################

.PHONY: all
all: tests

.PHONY: tests
tests: $(test_binaries)

.PHONY: test
test: tests
	./tests/test

.PHONY: clean
clean:
	rm -rf $(objects) $(mkdeps) $(gen) $(test_binaries)


%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MF "$(@:.o=.dep.mk)" -c $< -o $@


string.o: \
    $(LIBBASE)/size.h \
    $(LIBMAYBE)/def/maybe-size.h \
    $(LIBARRAY)/def/array-char.h \
    $(LIBARRAY)/array-char.h \
    $(LIBVEC)/def/vec-char.h \
    $(LIBVEC)/vec-char.h

tests/test: $(objects) $(gen_objects)

name_from_path = $(subst -,_,$1)

$(libbase_headers): $(LIBBASE)/%.h: $(LIBBASE)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libbase_sources): $(LIBBASE)/%.c: $(LIBBASE)/source.c.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libbase_objects): $(LIBBASE)/%.o: $(LIBBASE)/%.h

$(libmaybe_defs): $(LIBMAYBE)/def/maybe-%.h: $(LIBMAYBE)/def.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libarray_defs): $(LIBARRAY)/def/array-%.h: $(LIBARRAY)/def.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libarray_headers): $(LIBARRAY)/array-%.h: $(LIBARRAY)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libarray_sources): $(LIBARRAY)/array-%.c: $(LIBARRAY)/source.c.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) --sys-headers "libbase/$*.h" -o $@

$(libarray_objects): $(LIBARRAY)/array-%.o: \
    $(LIBARRAY)/array-%.h \
    $(LIBARRAY)/def/array-%.h \
    $(LIBBASE)/%.h \
    $(LIBBASE)/size.h \
    $(LIBMAYBE)/def/maybe-size.h

$(libvec_defs): $(LIBVEC)/def/vec-%.h: $(LIBVEC)/def.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libvec_headers): $(LIBVEC)/vec-%.h: $(LIBVEC)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(libvec_sources): $(LIBVEC)/vec-%.c: $(LIBVEC)/source.c.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) --sys-headers "libbase/$*.h" -o $@

$(libvec_objects): $(LIBVEC)/vec-%.o: \
    $(LIBVEC)/vec-%.h \
    $(LIBVEC)/def/vec-%.h \
    $(LIBBASE)/%.h \
    $(LIBBASE)/size.h \
    $(LIBARRAY)/def/array-%.h \
    $(LIBARRAY)/array-%.h \
    $(LIBMAYBE)/def/maybe-size.h


-include $(mkdeps)



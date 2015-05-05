
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


test_libbase_types  := char size
test_libmaybe_types := size
test_libarray_types := char
test_libvec_types   := char

char_type    := char
char_options := --typeclasses BOUNDED EQ ORD ENUM NUM \
                --extra num_type=signed char_funcs=true

size_type    := size_t
size_options := --typeclasses BOUNDED EQ ORD ENUM NUM \
                --extra num_type=unsigned

test_libbase_sources := $(foreach t,$(test_libbase_types),$(LIBBASE)/$t.c)
test_libbase_headers := $(test_libbase_sources:.c=.h)
test_libbase_objects := $(test_libbase_sources:.c=.o)

test_libmaybe_defs := $(foreach t,$(test_libmaybe_types),$(LIBMAYBE)/def/maybe-$t.h)

test_libarray_sources := $(foreach t,$(test_libarray_types),$(LIBARRAY)/array-$t.c)
test_libarray_headers := $(test_libarray_sources:.c=.h)
test_libarray_defs    := $(foreach t,$(test_libarray_types),$(LIBARRAY)/def/array-$t.h)
test_libarray_objects := $(test_libarray_sources:.c=.o)

test_libvec_sources := $(foreach t,$(test_libvec_types),$(LIBVEC)/vec-$t.c)
test_libvec_headers := $(test_libvec_sources:.c=.h)
test_libvec_defs    := $(foreach t,$(test_libvec_types),$(LIBVEC)/def/vec-$t.h)
test_libvec_objects := $(test_libvec_sources:.c=.o)

test_gen_objects := $(test_libbase_objects) \
                    $(test_libarray_objects) \
                    $(test_libvec_objects)

test_gen := $(test_libbase_sources) \
            $(test_libbase_headers) \
            $(test_libmaybe_defs) \
            $(test_libarray_sources) \
            $(test_libarray_headers) \
            $(test_libarray_defs) \
            $(test_libarray_objects) \
            $(test_libvec_sources) \
            $(test_libvec_headers) \
            $(test_libvec_defs) \
            $(test_libvec_objects) \
            $(test_gen_objects)

test_binaries := $(basename $(wildcard tests/*.c))

objects := string.o
mkdeps  := $(objects:.o=.dep.mk) $(test_gen_objects:.o=.dep.mk)



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
	rm -rf $(objects) $(mkdeps) $(test_gen) $(test_binaries)


%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MF "$(@:.o=.dep.mk)" -c $< -o $@


string.o: CPPFLAGS += -D_GNU_SOURCE
string.o: $(LIBBASE)/size.h \
          $(LIBMAYBE)/def/maybe-size.h \
          $(LIBARRAY)/def/array-char.h \
          $(LIBARRAY)/array-char.h \
          $(LIBVEC)/def/vec-char.h \
          $(LIBVEC)/vec-char.h

tests/test: $(objects) $(test_gen_objects)

name_from_path = $(subst -,_,$1)

$(test_libbase_headers): $(LIBBASE)/%.h: $(LIBBASE)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(test_libbase_sources): $(LIBBASE)/%.c: $(LIBBASE)/source.c.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(test_libbase_objects): $(LIBBASE)/%.o: $(LIBBASE)/%.h

$(test_libmaybe_defs): $(LIBMAYBE)/def/maybe-%.h: $(LIBMAYBE)/def.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(test_libarray_defs): $(LIBARRAY)/def/array-%.h: $(LIBARRAY)/def.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(test_libarray_headers): $(LIBARRAY)/array-%.h: $(LIBARRAY)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(test_libarray_sources): $(LIBARRAY)/array-%.c: $(LIBARRAY)/source.c.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) --sys-headers "libbase/$*.h" -o $@

$(test_libarray_objects): $(LIBARRAY)/array-%.o: \
    $(LIBARRAY)/array-%.h \
    $(LIBARRAY)/def/array-%.h \
    $(LIBBASE)/%.h \
    $(LIBBASE)/size.h \
    $(LIBMAYBE)/def/maybe-size.h

$(test_libvec_defs): $(LIBVEC)/def/vec-%.h: $(LIBVEC)/def.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(test_libvec_headers): $(LIBVEC)/vec-%.h: $(LIBVEC)/header.h.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) -o $@

$(test_libvec_sources): $(LIBVEC)/vec-%.c: $(LIBVEC)/source.c.jinja
	$(eval n := $(call name_from_path,$*))
	$(TPLRENDER) $< "$($(n)_type)" $($(n)_options) --sys-headers "libbase/$*.h" -o $@

$(test_libvec_objects): $(LIBVEC)/vec-%.o: \
    $(LIBVEC)/vec-%.h \
    $(LIBVEC)/def/vec-%.h \
    $(LIBBASE)/%.h \
    $(LIBBASE)/size.h \
    $(LIBARRAY)/def/array-%.h \
    $(LIBARRAY)/array-%.h \
    $(LIBMAYBE)/def/maybe-size.h


-include $(mkdeps)



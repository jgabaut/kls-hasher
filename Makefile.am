AUTOMAKE_OPTIONS = foreign
CFLAGS = @CFLAGS@
SHELL := /bin/bash
.ONESHELL:
MACHINE := $$(uname -m)
PACK_NAME = $(TARGET)-$(VERSION)-$(OS)-$(MACHINE)
kls_hasher_SOURCES = src/main.c
LDADD = $(HW_LDFLAGS)
AM_LDFLAGS = -O2
AM_CFLAGS = $(HW_CFLAGS) -O2 -Werror -Wpedantic -Wall
if DEBUG_BUILD
AM_LDFLAGS += -ggdb -O0
AM_CFLAGS += ""
else
AM_LDFLAGS += -s
endif

./anvil:
	@echo -e "\033[1;35m[Makefile]\e[0m    Bootstrapping \"./anvil\":"
	./scripts/bootstrap_anvil.sh repo_amboso
	@echo -e "\033[1;33mDone.\e[0m"

./src/anvil__kls_hasher.c:
	@echo -e "\033[1;34m[Makefile]\e[0m    Using \033[1;33manvil\e[0m to generate [\033[1;34m./src/anvil__kls_hasher.c\e[0m and \033[1;34m./src/anvil__kls_hasher.h\e[0m] for [ \033[1;36m$(VERSION)\e[0m ]:"
	-./anvil -qXG ./src $(VERSION)
	@echo -e "\033[1;33mDone.\e[0m"

$(TARGET): src/main.c
	@echo -e "    AM_CFLAGS: [ $(AM_CFLAGS) ]"
	@echo -e "    LDADD: [ $(LDADD) ]"
	./build.sh

clean:
	@echo -en "Cleaning build artifacts:  "
	-rm $(TARGET)
	-rm src/*.o
	-rm src/anvil__kls_hasher.*
	-rm static/*.o
	@echo -e "Done."
cleanob:
	@echo -en "Cleaning object build artifacts:  "
	-rm src/*.o
	-rm static/*.o
	@echo -e "Done."
anviltest:
	@echo -en "Running anvil tests."
	./anvil -tX
	@echo -e "Done."
all: ./anvil ./src/anvil__kls_hasher.c $(TARGET)
rebuild: clean all
.DEFAULT_GOAL := all

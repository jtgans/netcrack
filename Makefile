all: lib src tests            # Build lib src and tests.

targets:                      # Show a listing of targets.
	@echo Targets available:
	@echo ---------------------------------------------------------------------------
	@cat Makefile \
		|grep -e '^[a-zA-Z-]*:'               \
		|sed -e 's/[:#]/|/g' -e 's/\[/|[/g'   \
		|awk -F\| '{ printf("%12s -%-35s%s\n", $$1, $$3, $$4); }'

lib:                          # Build the project-shared library.
	make -C lib all

src: lib                      # Build source. [implies lib]
	make -C src all

tests: lib                    # Build tests. [implies lib]
	make -C tests all

run-tests: lib                # Run unit tests. [implies lib]
	make -C tests run

docs: Doxyfile                # Generate documentation
	doxygen

tags:                         # Generate ctags file.
	find -iname \*.[ch] |xargs ctags -T

clean:                        # Clean everything, including tags.
	make -C lib clean
	make -C src clean
	make -C tests clean
	make -C doc clean
	rm -f tags

mrclean: clean                # Clean more. [implies clean]
	make -C lib mrclean
	make -C src mrclean
	make -C tests mrclean
	make -C doc mrclean

.PHONY: lib tests clean docs

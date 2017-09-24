STATIC_SRC = __init__.py

EXTRA_DIST = $(STATIC_SRC)




## fixes for out-of-source builds, especially "make distcheck"

all-local: fix-out-of-source

FIXSOURCES = $(STATIC_SRC)

fix-out-of-source: $(FIXSOURCES)
	rm -f $@
	mkdir -p include
	for i in $^; do \
	  orig="$$i"; \
	  build="$$(basename $$i)"; \
	  if [[ "$${orig}" == *include* ]]; then build="include/$${build}"; fi; \
	  if [ ! -e $${build} ]; then cp $${orig} $${build}; \
	  echo $${build} >> $@; fi; \
	done
	touch $@

clean-local: fix-out-of-source
	if [ -e $< ]; then if [ -n "$$(cat $<)" ]; then rm -f $$(cat $<); fi; fi
	rm -f $<

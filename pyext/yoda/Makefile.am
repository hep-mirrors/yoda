SUBDIRS = include

EXTRA_DIST = \
    __init__.py \
    axis.pyx core.pyx declarations.pxd \
    util.pxd util.pyx \
    errors.cpp errors.hh \
    pyroot_helpers.hh merge.hh \
    rootcompat.pxd rootcompat.pyx \
    script_helpers.py \
EXTRA_DIST += \
    template.py \
    core.cpp core.h util.cpp

if WITH_CYTHON
core.cpp: core.pyx util.pxd util.pyx declarations.pxd
	./mktemplates
	cython core.pyx --cplus
util.cpp: util.pxd util.pyx
	./mktemplates
	cython util.pyx --cplus
else
core.cpp:
	@echo "Not (re)generating core.cpp since Cython is not installed"
util.cpp: util.pxd util.pyx
	@echo "Not (re)generating util.cpp since Cython is not installed"
endif


all-local: core.cpp util.cpp
	@true

clean-local:
	@rm -rf core.cpp core.h util.cpp

SUBDIRS = include

EXTRA_DIST = \
    __init__.py \
    axis.pyx core.pyx declarations.pxd \
    util.pxd util.pyx \
    errors.cpp errors.hh \
    pyroot_helpers.hh merge.hh \
    rootcompat.pxd rootcompat.pyx \
    script_helpers.py
EXTRA_DIST += \
    mktemplates \
    core.cpp core.h util.cpp


if WITH_CYTHON
core.cpp: core.pyx util.pxd util.pyx declarations.pxd
	./mktemplates
	cython core.pyx --cplus
else
core.cpp:
	@echo "Not (re)generating core.cpp since Cython is not installed"
endif


if WITH_CYTHON
util.cpp: util.pxd util.pyx
	./mktemplates
	cython util.pyx --cplus
else
util.cpp: util.pxd util.pyx
	@echo "Not (re)generating util.cpp since Cython is not installed"
endif


if ENABLE_ROOT
if WITH_CYTHON
rootcompat.cpp: util.pxd rootcompat.pxd rootcompat.pyx
	./mktemplates
	cython rootcompat.pyx --cplus
else
rootcompat.cpp: util.pxd rootcompat.pxd rootcompat.pyx
	@echo "Not (re)generating rootcompat.cpp since Cython is not installed"
endif
endif


if ENABLE_ROOT
all-local: core.cpp util.cpp rootcompat.cpp
	@true
else
all-local: core.cpp util.cpp
	@true
endif


distclean-local:
	rm -rf core.cpp core.h util.cpp rootcompat.cpp

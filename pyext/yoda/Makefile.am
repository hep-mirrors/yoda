EXTRA_DIST = \
    __init__.py \
    axis.pyx \
    errors.cpp errors.hh \
    pyroot_helpers.hh merge.hh \
    rootcompat.pxd rootcompat.pyx \
    script_helpers.py

CYTHON_SRC = \
    core.pyx \
    declarations.pxd \
    util.pyx \
    util.pxd \
    include/AnalysisObject.pyx \
    include/Axis1D_BIN1D_DBN.pyx \
    include/Axis2D_BIN2D_DBN.pyx \
    include/Bin.pyx \
    include/Bin1D_DBN.pyx \
    include/Bin2D_DBN.pyx \
    include/Dbn1D.pyx \
    include/Dbn2D.pyx \
    include/Dbn3D.pyx \
    include/Errors.pyx \
    include/Histo1D.pyx \
    include/Histo2D.pyx \
    include/HistoBin1D.pyx \
    include/HistoBin2D.pyx \
    include/IO.pyx \
    include/Point2D.pyx \
    include/Point3D.pyx \
    include/Profile1D.pyx \
    include/Profile2D.pyx \
    include/ProfileBin1D.pyx \
    include/ProfileBin2D.pyx \
    include/Scatter2D.pyx


EXTRA_DIST += \
    $(CYTHON_SRC) \
    mktemplates \
    core.cpp core.h util.cpp


if WITH_CYTHON
core.cpp: $(CYTHON_SRC)
	./mktemplates
	cython core.pyx --cplus
else
core.cpp: $(CYTHON_SRC)
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


## Always include rootcompat.cpp in the dist... only developers should be building dist tarballs
EXTRA_DIST += rootcompat.cpp

if ! ENABLE_ROOT
all-local: core.cpp util.cpp
	@true

else

all-local: core.cpp util.cpp rootcompat.cpp
	@true
if WITH_CYTHON
rootcompat.cpp: util.pxd rootcompat.pxd rootcompat.pyx
	./mktemplates
	cython rootcompat.pyx --cplus
else
rootcompat.cpp: util.pxd rootcompat.pxd rootcompat.pyx
	@echo "Not (re)generating rootcompat.cpp since Cython is not installed"
endif
endif


distclean-local:
	rm -f core.cpp core.h util.cpp rootcompat.cpp include/generated/*

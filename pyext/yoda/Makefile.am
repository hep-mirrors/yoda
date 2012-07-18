all-local: yoda.cpp
	@true

yoda.cpp: yoda.pyx
	$(PYTHON) -m cython --cplus yoda.pyx

yoda.pyx: include/00-imports.pyx include/10-AnalysisObject.pyx \
	include/20-Dbn1D.pyx include/20-Dbn2D.pyx include/20-Point2D.pyx include/20-Point3D.pyx \
	include/30-HistoBin1D.pyx include/30-ProfileBin1D.pyx include/30-HistoBin2D.pyx \
	include/30-Reader.pyx \
	include/30-Scatter2D.pyx \
	include/40-Histo1D.pyx include/40-Profile1D.pyx \
	include/40-Histo2D.pyx \
	include/40-WriterYODA.pyx include/40-WriterAIDA.pyx \
	include/99-ReaderYODA.pyx include/99-ReaderAIDA.pyx
	cat $+ > yoda.pyx
#include/30-Scatter3D.pyx
#include/40-Profile2D.pyx

clean-local:
	rm yoda.cpp yoda.pyx

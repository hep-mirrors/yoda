all-local:
	cat imports.pyx include/*.pyx > yoda.pyx
	$(PYTHON) -m cython --cplus yoda.pyx	

clean-local:
	rm yoda.cpp yoda.pyx

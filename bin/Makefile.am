dist_bin_SCRIPTS = yoda-config yoda2aida yoda2flat aida2yoda aida2flat

## bash completion
if ENABLE_PYEXT
dist_pkgdata_DATA = yoda-completion
bashcomp_dir = $(prefix)/etc/bash_completion.d
install-data-local:
	if [[ -d "$(bashcomp_dir)" && -w "$(bashcomp_dir)" ]]; then \
      install --mode 644 yoda-completion $(bashcomp_dir)/; fi
uninstall-local:
	rm -f $(bashcomp_dir)/yoda-completion
else
EXTRA_DIST = yoda-completion
endif

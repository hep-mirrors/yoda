dist_bin_SCRIPTS = yoda-config

if ENABLE_PYEXT
dist_bin_SCRIPTS += \
  yodacnv yodamerge yodadiff yodals yodahist yodaplot yodascale \
  yoda2aida yoda2flat aida2yoda aida2flat flat2yoda
if ENABLE_ROOT
dist_bin_SCRIPTS += yoda2root
endif
endif

## bash completion
dist_pkgdata_DATA = yoda-completion
bashcomp_dir = $(DESTDIR)$(prefix)/etc/bash_completion.d
install-data-local:
	if [[ -d "$(bashcomp_dir)" && -w "$(bashcomp_dir)" ]]; then \
      $(install_sh_DATA) yoda-completion $(bashcomp_dir)/; fi
uninstall-local:
	rm -f $(bashcomp_dir)/yoda-completion

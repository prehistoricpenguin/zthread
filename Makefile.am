
## extra files
EXTRA_DIST = \
README.1ST \
BUILDING \
THANK.YOU \
LGPL.TXT \
ZTHREAD_2_2_8.dsw \
ZTHREAD_2_2_8_shared.dsp \
ZTHREAD_2_2_8_static.dsp \
depcomp


## install the config script
install-exec-hook:
	$(mkinstalldirs) $(bindir)
	$(INSTALL_PROGRAM) $(top_srcdir)/share/zthread-config $(bindir)

## install the config script & m4 macros
install-data-hook:
	$(mkinstalldirs) $(datadir)/aclocal
	$(INSTALL_DATA) $(top_srcdir)/share/zthread.m4 $(datadir)/aclocal
	$(INSTALL_DATA) $(top_srcdir)/share/pthread.m4 $(datadir)/aclocal
	mkdir -p $(DESTDIR)$(includedir)/zthread
	cp -pR $(top_srcdir)/include/zthread $(DESTDIR)$(includedir)/

## uninstall the config script & m4 macros
uninstall-local:
	-rm -rf $(datadir)/aclocal/pthread.m4
	-rm -rf $(datadir)/aclocal/zthread.m4

## include the doc & share directories in the distribution
dist-hook:
	cp -pR $(top_srcdir)/doc $(distdir)
	cp -pR $(top_srcdir)/share $(distdir)
	cp -pR $(top_srcdir)/contrib $(distdir)
	cp -pR $(top_srcdir)/include $(distdir)
	cp -pR $(top_srcdir)/src/*.h $(distdir)/src
	cp -pR $(top_srcdir)/src/posix $(distdir)/src
	cp -pR $(top_srcdir)/src/linux $(distdir)/src
	cp -pR $(top_srcdir)/src/macos $(distdir)/src
	cp -pR $(top_srcdir)/src/solaris $(distdir)/src
	cp -pR $(top_srcdir)/src/win32 $(distdir)/src
	cp -pR $(top_srcdir)/src/win9x $(distdir)/src
	cp -pR $(top_srcdir)/src/vanilla $(distdir)/src


## RPM from the distribution rule
rpm: dist
	rpm -ta $(PACKAGE)-$(VERSION).tar.gz
	mv -f /usr/src/redhat/SRPMS/$(PACKAGE)-$(VERSION)-*.rpm .
	mv -f /usr/src/redhat/RPMS/*/$(PACKAGE)-$(VERSION)-*.rpm .
	mv -f /usr/src/redhat/RPMS/*/$(PACKAGE)-devel-$(VERSION)-*.rpm .
	mv -f /usr/src/redhat/RPMS/*/$(PACKAGE)-doc-$(VERSION)-*.rpm .

## Build the documentation
docs: 
	if test x$(DOXYGEN) != x; then\
	-rm -f $(top_srcdir)/doc/html/*; \
	$(DOXYGEN) $(top_srcdir)/doc/zthread.doxygen; \
	else \
	echo; \
	echo "This configuration requires Doxygen, by Dimitri van Heesch."; \
	echo "Visit www.stack.nl/~dimitri/doxygen/ for information about how to"; \
	echo "obtain a copy. If you already have Doxygen and it was not detected"; \
	echo "you should try the --with-doxygen option."; \
	echo; \
	fi; 



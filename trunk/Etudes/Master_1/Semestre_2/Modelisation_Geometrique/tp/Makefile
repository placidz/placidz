# -------------------------------------------------------------------
# Binaries
SUBDIRS	= tp*
# -------------------------------------------------------------------

# -------------------------------------------------------------------
# Binaries
ECHO	= /bin/echo
CD		= cd
# -------------------------------------------------------------------


# -------------------------------------------------------------------
# Rules

all:
	@for d in $(SUBDIRS); do \
	$(ECHO) "~ Making all in '$$d'"; \
	($(CD) $$d > /dev/null && $(MAKE) all); done

clean :
	@for d in $(SUBDIRS); do \
	$(ECHO) "~ Making clean in '$$d'"; \
	($(CD) $$d > /dev/null && $(MAKE) clean); done

clean-all :
	@for d in $(SUBDIRS); do \
	$(ECHO) "~ Making clean-all in '$$d'"; \
	($(CD) $$d > /dev/null && $(MAKE) clean-all); done
# -------------------------------------------------------------------

#################################################################################
# ___  ___        _            __  _  _                                         #
# |  \/  |       | |          / _|(_)| |                                        #
# | .  . |  __ _ | | __  ___ | |_  _ | |  ___                                   #
# | |\/| | / _` || |/ / / _ \|  _|| || | / _ \                                  #
# | |  | || (_| ||   < |  __/| |  | || ||  __/                                  #
# \_|  |_/ \__,_||_|\_\ \___||_|  |_||_| \___|                                  #
#                                                                               #
#  Copyright (c) 2010                                                           #
#  Author: Dimitri KUDELSKI                                                     #
#  Contact: kudelski [at] univmed [dot] fr                                      #
#                                                                               #
#                                                                               #
# This program is free software: you can redistribute it and/or modify it under #
# the terms of the GNU General Public License as published by the Free Software #
# Foundation, either version 3 of the License, or (at your option) any later    #
# version.                                                                      #
#                                                                               #
# This program is distributed in the hope that it will be useful, but WITHOUT   #
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS #
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more         #
# details.                                                                      #
#                                                                               #
#################################################################################


# -------------------------------------------------------------------
# Directories
SUBDIRS  = gmtools tp
# -------------------------------------------------------------------

# -------------------------------------------------------------------
# Binaries
ECHO	= /bin/echo
CD		= cd
ZIP		= zip
FIND	= find
# -------------------------------------------------------------------


# -------------------------------------------------------------------
# Rules

all:
	@for d in $(SUBDIRS); do \
	$(ECHO) "~ Making all in '$$d'"; \
	($(CD) $$d > /dev/null && $(MAKE) all); done

clean :
	@for d in $(INSTALLS) $(SUBDIRS); do \
	$(ECHO) "~ Making clean in '$$d'"; \
	($(CD) $$d > /dev/null && $(MAKE) clean); done

clean-all :
	@for d in $(INSTALLS) $(SUBDIRS); do \
	$(ECHO) "~ Making clean-all in '$$d'"; \
	($(CD) $$d > /dev/null && $(MAKE) clean-all); done

arch :
	@$(ECHO) "~ Archiving the directory"
	@$(ZIP) -rq "$(PWD)"-`date +%Y%m%d-%H%M`.zip .

touch :
	@$(ECHO) "~ Touching all files"
	@$(FIND) . -iname "*" -exec touch {} \;
# -------------------------------------------------------------------

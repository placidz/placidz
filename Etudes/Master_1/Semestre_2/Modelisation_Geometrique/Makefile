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
# -------------------------------------------------------------------
DIRS		= gmtools tp
# -------------------------------------------------------------------


# -------------------------------------------------------------------
# Rules
# -------------------------------------------------------------------
all clean:
	@for d in $(DIRS); do \
	echo "~ Making $@ in '$$d'"; \
	(cd $$d > /dev/null; $(MAKE) $@); \
	done


arch :
	@echo "~ Archiving the directory"
	@zip -rq "$(PWD)"-`date +%Y%m%d-%H%M`.zip .


touch :
	@echo "~ Touching all files"
	@find . -iname "*" -exec touch {} \;
# -------------------------------------------------------------------

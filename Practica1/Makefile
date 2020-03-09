#### Configuration ################################################################################

_PROJ   = concurrent_pi#                     # The name of the project and generated executable
_SDIR   = src#                                 # Path to directory of source files (relative to ./)
_HDIR   = include#                             # Path to directory of header files (relative to ./)
_BDIR   = bin#                                 # Path to directory of binary files (relative to ./)
_SSUF   = c#                                 # Suffix of source files
_HSUF   = h#                                 # Suffix of header files
_CC     = gcc#                                 # Compiler to be used
_CFLAGS = -Wall -Wextra -pedantic#             # Compilation flags
_SCRIPT = :#                                   # Any shell script to run before build (replace ':')

SHELL   = /bin/bash#                           # Shell to be used by makefile
CARGS   = -I $(_HDIR) $(_CFLAGS)#              # Full set of compiler arguments
PURPLE  = \033[0;35m#                          # Encoding of purple color for terminal output
CYAN    = \033[0;36m#                          # Encoding of cyan color for terminal output
NC      = \033[0m#

##### File Lists ##################################################################################

# A list of header files on which source files depend
HEDRS   = $(shell find $(_HDIR) -print | grep .$(_HSUF))

# A list of object files on which their existing source files depend
OBJS    = $(shell find $(_SDIR) -print | grep .$(_SSUF) | \
                  sed -r "s/($(_SDIR))\/(.*)\.($(_SSUF))/$(_BDIR)\/obj\/\2\.o/")

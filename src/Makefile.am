include $(top_srcdir)/fragment.am

bin_PROGRAMS = gameSL
gameSL_SOURCES = ./FileBridge.cpp \
	       ./keyboard.c \
               ./main.c \
               ./screen/ncurses_main.c \
               ./screen/ncurses_theme.c \
               ./ter/consle.c \
		./ter/input.c \
		./ter/message.c 
gameSL_LDADD = $(top_builddir)/lib/nbt/libnbt.a \
    -L$(top_builddir)/lib/other_tools -lother_tools \
    -lncurses    


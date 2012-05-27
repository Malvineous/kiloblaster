bin_PROGRAMS = kfile1 kfile2 kfile3

CORE_FILES = port.cpp
CORE_FILES += dbopl.cpp
CORE_FILES += 2b_editr.cpp
CORE_FILES += 2b_info.cpp
CORE_FILES += 2b_objmn.cpp
CORE_FILES += 2b_objs.cpp
CORE_FILES += 2b_playr.cpp
CORE_FILES += 2blaster.cpp
CORE_FILES += config.cpp
CORE_FILES += gamectrl.cpp
CORE_FILES += graphics.cpp
CORE_FILES += music.cpp
CORE_FILES += shapeman.cpp
CORE_FILES += windows.cpp

EXTRA_CORE_FILES = port.h
EXTRA_CORE_FILES += dbopl.h
EXTRA_CORE_FILES += dosbox.h
EXTRA_CORE_FILES += adlib.h
EXTRA_CORE_FILES += include/2blaster.h
EXTRA_CORE_FILES += include/begin.h
EXTRA_CORE_FILES += include/config.h
EXTRA_CORE_FILES += include/copyfile.h
EXTRA_CORE_FILES += include/gamectrl.h
EXTRA_CORE_FILES += include/gr.h
EXTRA_CORE_FILES += include/keyboard.h
EXTRA_CORE_FILES += include/music.h
EXTRA_CORE_FILES += include/pixwrite.h
EXTRA_CORE_FILES += include/uncrunch.h
EXTRA_CORE_FILES += include/windows.h
EXTRA_CORE_FILES += include/worx.h

kfile1_SOURCES = $(CORE_FILES) 2b_vol1.cpp
kfile2_SOURCES = $(CORE_FILES) 2b_vol2.cpp
kfile3_SOURCES = $(CORE_FILES) 2b_vol3.cpp

EXTRA_kfile1_SOURCES = $(EXTRA_CORE_FILES)
EXTRA_kfile2_SOURCES = $(EXTRA_CORE_FILES)
EXTRA_kfile3_SOURCES = $(EXTRA_CORE_FILES)

AM_CPPFLAGS = $(sdl_CFLAGS) -Wno-write-strings -fpermissive $(libgamemusic_CFLAGS)
AM_LDFLAGS = $(sdl_LIBS) $(libgamemusic_LIBS)

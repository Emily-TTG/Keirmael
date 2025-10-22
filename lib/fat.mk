FAT_DIRECTORY = lib/fat
FAT_SOURCES = $(wildcard $(FAT_DIRECTORY)/*.c)
FAT_OBJECTS = $(FAT_SOURCES:.c=.%.o)

lib/libfat.%.a: $(FAT_OBJECTS) ; ${$*_library}

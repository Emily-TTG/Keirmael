HOST_CC = $(CC)
HOST_AR = $(AR)

define host_library
	$(AR)$% rc $@ $^
endef

%.host.o: %.c
	$(HOST_CC) -c $< -o $@

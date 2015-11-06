SUBDIR = ./client ./server
all: $(SUBDIR)

$(SUBDIR):
	@$(MAKE) --no-print-directory -C $@

.PHONY: $(SUBDIR)

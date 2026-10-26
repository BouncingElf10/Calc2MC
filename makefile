NAME = MATH
ICON = iconmc.png
DESCRIPTION = "Math solver with steps"
COMPRESSED = YES
ARCHIVED = YES
CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

FONTDIR = $(SRCDIR)/fonts
FONTS = $(FONTDIR)/body.inc $(FONTDIR)/small.inc
DEPS = $(FONTS)

include $(shell cedev-config --makefile)

$(FONTDIR)/%.inc: $(FONTDIR)/%.convfont
	$(Q)convfont -o carray -t $< $@

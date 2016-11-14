CFLAGS=-I $(WIN32EX)/include

STATIC_LIB=$(WIN32EX)/lib/libWin32Ex$(OUTPUT_SUFFIX)$(STATIC_EXT)
SHARED_LIB=$(WIN32EX)/lib/libWin32Ex$(OUTPUT_SUFFIX)$(SHARED_EXT)

FILES=misc \
      rtarray \
      rtbinarysearch \
      rtchar \
      rterror \
      rtlinkedlist \
      rtmemory \
      rtquicksort \
      rtruntimeheap \
      rtsortablearray

OBJECTS=$(addsuffix $(OBJ_EXT),$(addprefix $(OUTPUT_DIR)/,$(FILES)))

$(STATIC_LIB) : $(OBJECTS)
	$(LD_STATIC) $@ $(OBJECTS)

$(OUTPUT_DIR)/%$(OBJ_EXT) : $(ROOT)/src/%.c $(WIN32EX)/include/rtwin32ex.h
	@$(MK_DIR) $(subst /$(notdir $@),,$@)
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(STATIC_LIB)

release: $(STATIC_LIB)

clean:
	$(RM_DIR) $(ROOT)/obj/Debug
	$(RM_DIR) $(ROOT)/obj/Release
	$(RM_FILE) $(ROOT)/lib/*$(STATIC_EXT)

.PHONY: debug release clean


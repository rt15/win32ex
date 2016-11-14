CFLAGS=-I $(WIN32EX)/include

PROJECTS=Tests \
         Console

PROJECTS_EXECUTABLES=$(addsuffix $(OUTPUT_SUFFIX)$(EXE_EXT),$(addprefix $(ROOT)/bin/,$(PROJECTS)))

OBJECTS=$(addsuffix $(OBJ_EXT),$(addprefix $(OUTPUT_DIR)/,$(FILES)))

$(ROOT)/bin/%$(OUTPUT_SUFFIX)$(EXE_EXT) : $(OUTPUT_DIR)/%$(OBJ_EXT)
	$(LD_EXE) $< $(LDFLAGS) -o $@

$(OUTPUT_DIR)/%$(OBJ_EXT) : $(ROOT)/src/%.c $(WIN32EX)/include/rtwin32ex.h
	@$(MK_DIR) $(subst /$(notdir $@),,$@)
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(PROJECTS_EXECUTABLES)

release: $(PROJECTS_EXECUTABLES)

clean:
	$(RM_DIR) $(ROOT)/obj/Debug
	$(RM_DIR) $(ROOT)/obj/Release
	$(RM_FILE) $(ROOT)/bin/*$(EXE_EXT)

.SECONDARY: $(OBJECTS)

.PHONY: debug release clean


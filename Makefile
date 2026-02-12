# ┌───────────────────────────────────────────────────────────────────────────┐
# │ AccelTherm — zero-maintenance Makefile                                    │
# │ - Auto-discovers all .cpp files (no edits needed when adding files)       │
# │ - Auto-generates header deps (-MMD -MP)                                   │
# │ - Pretty, colored, “fancy” output + help + status   					  |
# │ - Developed by Benyamin Eslami in Feb 2025 - benyamin.eslami@wsu.edu      │	
# └───────────────────────────────────────────────────────────────────────────┘

SHELL := /bin/bash

# ===== User-tunable knobs =====================================================
CXX       ?= g++
STD       ?= c++20
OPT       ?= -O2
DBG       ?= -g
WARN      ?= -Wall -Wextra -Wpedantic
DEFS      ?=
EXTRA     ?=

TARGET    ?= AccelTherm
SRC_DIRS  ?= .
BUILD_DIR ?= build

LDFLAGS   ?=
LDLIBS    ?=

# Verbosity: V=1 shows full commands
V ?= 0

# ===== Colors / styles ========================================================
RESET  := \033[0m
BOLD   := \033[1m
DIM    := \033[2m
RED    := \033[31m
GREEN  := \033[32m
YELLOW := \033[33m
BLUE   := \033[34m
MAG    := \033[35m
CYAN   := \033[36m

# ===== Pretty logging macros ==================================================
ifeq ($(V),0)
  Q := @
  define say
    @printf "$(1)\n"
  endef
else
  Q :=
  define say
    @true
  endef
endif

define banner
  @printf "$(BOLD)$(CYAN)┌──────────────────────────────────────────────────────────────┐$(RESET)\n"
  @printf "$(BOLD)$(CYAN)│$(RESET)  $(BOLD)AccelTherm Build$(RESET)  $(DIM)(auto-discovery, deps, pretty)$(RESET)\n"
  @printf "$(BOLD)$(CYAN)└──────────────────────────────────────────────────────────────┘$(RESET)\n"
endef

define kv
  @printf "  $(BOLD)$(1)$(RESET) $(DIM)→$(RESET) $(2)\n"
endef

# ===== Compiler flags =========================================================
CPPFLAGS += $(DEFS)
CXXFLAGS += -std=$(STD) $(OPT) $(DBG) $(WARN) $(EXTRA) -MMD -MP
# Include dirs: auto-collect directories containing headers
INC_DIRS := $(sort $(dir $(shell find $(SRC_DIRS) -type f \( -name '*.h' -o -name '*.hpp' \))))
CPPFLAGS += $(addprefix -I,$(INC_DIRS))

# ===== Sources / objects / deps ==============================================
SOURCES := $(shell find $(SRC_DIRS) -type f -name '*.cpp')
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS    := $(OBJECTS:.o=.d)

# ===== Progress counters ======================================================
TOTAL := $(words $(OBJECTS))
N := 0
define step
  $(eval N := $(shell echo $$(( $(N) + 1 ))))
  @printf "$(DIM)[$(N)/$(TOTAL)]$(RESET) "
endef

# ===== Targets ================================================================
.PHONY: all clean run help status tree print

all: status $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	$(call say,$(BOLD)$(MAG)🔗 Linking$(RESET) $(DIM)$@$(RESET))
	$(Q)$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(call say,$(BOLD)$(GREEN)✅ Built$(RESET) $(BOLD)$@$(RESET))

# Compile
$(BUILD_DIR)/%.o: %.cpp
	$(Q)mkdir -p $(dir $@)
	$(call step)
	$(call say,$(BOLD)$(BLUE)🛠  Compile$(RESET) $(DIM)$<$(RESET))
	$(Q)$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Auto-include dependency files
-include $(DEPS)

# ===== Fancy printing =========================================================
help:
	$(call banner)
	@printf "$(BOLD)$(YELLOW)Targets$(RESET)\n"
	@printf "  $(BOLD)make$(RESET) / $(BOLD)make all$(RESET)      Build $(TARGET)\n"
	@printf "  $(BOLD)make run$(RESET)           Run ./$(TARGET)\n"
	@printf "  $(BOLD)make clean$(RESET)         Remove build artifacts\n"
	@printf "  $(BOLD)make status$(RESET)        Show configuration & file counts\n"
	@printf "  $(BOLD)make tree$(RESET)          Show discovered source file tree\n"
	@printf "  $(BOLD)make print$(RESET)         Print expanded variables\n"
	@printf "\n$(BOLD)$(YELLOW)Variables$(RESET)\n"
	@printf "  $(BOLD)V=1$(RESET)               Verbose commands\n"
	@printf "  $(BOLD)CXX=clang++$(RESET)        Choose compiler\n"
	@printf "  $(BOLD)TARGET=name$(RESET)        Output binary name\n"
	@printf "  $(BOLD)STD=c++23$(RESET)          Language standard\n"
	@printf "  $(BOLD)OPT=-O3$(RESET)            Optimization flags\n"
	@printf "  $(BOLD)DBG=-g0$(RESET)            Debug flags\n"
	@printf "  $(BOLD)LDLIBS+=-lm$(RESET)        Link libraries\n"

status:
	$(call banner)
	$(call kv,Target,$(BOLD)$(TARGET)$(RESET))
	$(call kv,Compiler,$(CXX))
	$(call kv,Std,-std=$(STD))
	$(call kv,Build dir,$(BUILD_DIR)/)
	$(call kv,Source roots,$(SRC_DIRS))
	$(call kv,CPP files,$(BOLD)$(words $(SOURCES))$(RESET))
	$(call kv,Header dirs,$(BOLD)$(words $(INC_DIRS))$(RESET))
	@printf "  $(BOLD)Flags$(RESET) $(DIM)→$(RESET) $(CXXFLAGS)\n"
	@printf "\n"

tree:
	$(call banner)
	@printf "$(BOLD)$(YELLOW)Discovered .cpp files$(RESET)\n"
	@for f in $(SOURCES); do \
	  printf "  $(CYAN)•$(RESET) %s\n" "$$f"; \
	done

print:
	$(call banner)
	@printf "$(BOLD)$(YELLOW)Expanded variables$(RESET)\n"
	@printf "  $(BOLD)SOURCES$(RESET) =\n"; \
	for f in $(SOURCES); do printf "    $(DIM)-$(RESET) %s\n" "$$f"; done
	@printf "  $(BOLD)OBJECTS$(RESET) =\n"; \
	for o in $(OBJECTS); do printf "    $(DIM)-$(RESET) %s\n" "$$o"; done
	@printf "  $(BOLD)CPPFLAGS$(RESET) = $(CPPFLAGS)\n"
	@printf "  $(BOLD)CXXFLAGS$(RESET) = $(CXXFLAGS)\n"
	@printf "  $(BOLD)LDFLAGS$(RESET)  = $(LDFLAGS)\n"
	@printf "  $(BOLD)LDLIBS$(RESET)   = $(LDLIBS)\n"

run: $(TARGET)
	$(call say,$(BOLD)$(GREEN)▶ Running$(RESET) $(BOLD)./$(TARGET)$(RESET))
	$(Q)./$(TARGET)

clean:
	$(call say,$(BOLD)$(RED)🧹 Cleaning$(RESET) $(DIM)$(BUILD_DIR)/ $(TARGET)$(RESET))
	$(Q)rm -rf $(BUILD_DIR) $(TARGET)
	$(call say,$(BOLD)$(GREEN)✅ Clean$(RESET))

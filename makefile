SHELL := /bin/bash

NAME := scop
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
DEPS_DIR := deps
GLFW_DIR := glfw
GLAD_DIR := glad
GLAD_FILE := $(OBJ_DIR)/glad.o
RESOURCE_DIR := resources
TEST_FILE := $(RESOURCE_DIR)/objFiles/cubeTexture.obj
TESTER := bin/tester_arg_parse.sh
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SOURCES:.cpp=.o))
DEPS := $(patsubst $(SRC_DIR)%,$(DEPS_DIR)%,$(SOURCES:.cpp=.d)) $(patsubst $(OBJ_DIR)%,$(DEPS_DIR)%,$(GLAD_FILE:.o=.d))
DEBUG := 0
# codam computer wants clang (c++) with g++ it doesn't link the glfw libraries
CC := c++
INC_FLAGS := -I$(INC_DIR) -I$(GLFW_DIR)/include -I$(GLAD_DIR)/include
LIBS_FLAGS := -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglfw
CPP_FLAGS := -Wall -Wextra -Werror -Wshadow -Wpedantic -std=c++17
ifeq ($(DEBUG),1)
    CPP_FLAGS += -fsanitize=address -g3
endif
DEP_FLAGS = -MMD -MF $(DEPS_DIR)/$*.d

GREEN := \x1b[32;01m
RED := \x1b[31;01m
BLUE := \x1b[34;01m
RESET := \x1b[0m

all: $(GLFW_DIR) $(GLAD_DIR) $(NAME)

run: all
	@clear
	@LD_LIBRARY_PATH="" ./$(NAME) --file $(TEST_FILE)

# building glfw
$(GLFW_DIR):
	@unzip -a $(RESOURCE_DIR)/glfw-3.4.zip -d .
	@mv glfw-3.4 $(GLFW_DIR)
	@mkdir -p $(GLFW_DIR)/build
	@cmake -S $(GLFW_DIR) -B $(GLFW_DIR)/build
	@$(MAKE) --no-print-directory -j4 -C $(GLFW_DIR)/build
	@printf "(scop) $(GREEN)Built glfw in $$(pwd)/$(GLFW_DIR)$(RESET)\n"

# extracting GLAD
$(GLAD_DIR):
	@mkdir -p $(GLAD_DIR)
	@unzip -a $(RESOURCE_DIR)/glad.zip -d $(GLAD_DIR)
	@printf "(scop) $(GREEN)Extracted GLAD in $$(pwd)/$(GLAD_DIR)$(RESET)\n"

$(NAME): $(OBJECTS) $(GLAD_FILE)
	@$(CC) $(CPP_FLAGS) $(INC_FLAGS) $(LIBS_FLAGS) $^ -o $@
	@printf "(scop) $(GREEN)Created executable $@$(RESET)\n"

$(OBJ_DIR) $(DEPS_DIR):
	@mkdir -p $@

-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp makefile | $(DEPS_DIR) $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(subst $(OBJ_DIR),$(DEPS_DIR),$(dir $@))
	@$(CC) $(CPP_FLAGS) $(INC_FLAGS) $(DEP_FLAGS) -c $< -o $@
	@printf "(scop) $(BLUE)Created object $$(basename $@)$(RESET)\n"

# glad file is a C file, has to be compiled separatedly
$(GLAD_FILE): $(patsubst $(OBJ_DIR)%,$(GLAD_DIR)/src%,$(GLAD_FILE:.o=.c)) | $(DEPS_DIR) $(OBJ_DIR)
	@gcc -Wall -Wextra -Werror -MMD -MF $(DEPS_DIR)/glad.d -I$(GLAD_DIR)/include -c $< -o $@
	@printf "(scop) $(BLUE)Created object $$(basename $@)$(RESET)\n"

test: all
	@clear
	@./$(TESTER)

clean:
	@rm -f $(NAME)
	@printf "(scop) $(RED)Removed executable $(NAME)$(RESET)\n"
	@rm -rf $(OBJECTS) $(GLAD_FILE)
	@printf "(scop) $(RED)Removed object files $(OBJECTS) $(GLAD_FILE)$(RESET)\n"
	@rm -rf $(DEPS)
	@printf "(scop) $(RED)Removed dependencies $(DEPS) $(RESET)\n"

fclean: clean
	@rm -rf $(GLFW_DIR)
	@printf "(scop) $(RED)Removed folder $$(pwd)/$(GLFW_DIR)$(RESET)\n"
	@rm -rf $(GLAD_DIR)
	@printf "(scop) $(RED)Removed folder $$(pwd)/$(GLAD_DIR)$(RESET)\n"

re: fclean all

.PHONY: all run clean fclean re

.DEFAULT_GOAL:=all
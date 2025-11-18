SHELL := /bin/bash

NAME := scop
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := inc
DEP_DIR := deps
GLFW_DIR := glfw
GLAD_DIR := glad
RESOURCE_DIR := resources
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SOURCES:.cpp=.o))
DEPS := $(patsubst $(SRC_DIR)%,$(DEP_DIR)%,$(SOURCES:.cpp=.d))

CC := c++
INC_FLAGS := -I$(INC_DIR) -I$(GLFW_DIR)/include -I$(GLAD_DIR)/include
LIBS_FLAGS := -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglfw
CPP_FLAGS := -Wall -Wextra -Werror -Wshadow -Wpedantic -std=c++17 -g3
DEP_FLAGS = -MMD -MF $(DEP_DIR)/$*.d

GREEN := \x1b[32;01m
RED := \x1b[31;01m
BLUE := \x1b[34;01m
RESET := \x1b[0m

all: $(GLFW_DIR) $(GLAD_DIR) $(NAME)

run: $(NAME)
	@clear
	@./$(NAME)

# building glfw
$(GLFW_DIR):
	@unzip -a $(RESOURCE_DIR)/glfw-3.4.zip -d .
	@mv glfw-3.4 $(GLFW_DIR)
	@mkdir -p $(GLFW_DIR)/build
	@cmake -S $(GLFW_DIR) -B $(GLFW_DIR)/build
	@$(MAKE) -j4 -C $(GLFW_DIR)/build
	@printf "(scop) $(GREEN)Built glfw in $$(pwd)/$(GLFW_DIR)$(RESET)\n"

# extracting GLAD
$(GLAD_DIR):
	@mkdir -p $(GLAD_DIR)
	@unzip -a $(RESOURCE_DIR)/glad.zip -d $(GLAD_DIR)
	@mv $(GLAD_DIR)/src/glad.c $(SRC_DIR)/
	@rmdir $(GLAD_DIR)/src
	@printf "(scop) $(GREEN)Extracted GLAD in $$(pwd)/$(GLAD_DIR)$(RESET)\n"

# on Codam computer: unset LD_LIBRARY_PATH && LD_LIBRARY_PATH=""
$(NAME): $(OBJECTS) 
	@$(CC) $(CPP_FLAGS) $(INC_FLAGS) $(LIBS_FLAGS) $^ -o $@
	@printf "(scop) $(GREEN)Created executable $@$(RESET)\n"

$(OBJ_DIR) $(DEP_DIR):
	@mkdir -p $@

-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp makefile | $(DEP_DIR) $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@mkdir -p $(subst $(OBJ_DIR),$(DEP_DIR),$(dir $@))
	@$(CC) $(CPP_FLAGS) $(INC_FLAGS) $(DEP_FLAGS) -c $< -o $@
	@printf "(scop) $(BLUE)Created object $$(basename $@)$(RESET)\n"

clean:
	@rm -rf $(OBJECTS)
	@printf "(scop) $(RED)Removed object files $(OBJECTS)$(RESET)\n"
	@rm -rf $(DEPS)
	@printf "(scop) $(RED)Removed dependencies $(DEPS)$(RESET)\n"

fclean: clean
	@rm -rf $(GLFW_DIR)
	@printf "(scop) $(RED)Removed folder $$(pwd)/$(GLFW_DIR)$(RESET)\n"
	@rm -rf $(GLAD_DIR)
	@printf "(scop) $(RED)Removed folder $$(pwd)/$(GLAD_DIR)$(RESET)\n"

re: fclean all

.PHONY: all run client clean fclean re

.DEFAULT_GOAL:=all
#
# Created by Aaron Kampmeier on 7/6/2020.
# Copyright © 2020 Aaron Kampmeier. All rights reserved.
# ASU ID: 1217750807
# For Class: CSE 240 with Prof. Selgrad
#

# A primitive makefile to be used for the graders of Assignment 4 to build my application on general.asu.edu
# Requires that there exist an "out" directory

PROJ_NAME = Assignment_4
BUILD_DIR = out
CPP = g++
C = gcc
FLAGS = -g -Wall
OBJS = $(BUILD_DIR)/FrequencyAnalyzer.o
MAIN = $(BUILD_DIR)/kampmeier_aaron_hw4.o
CXX_INCLUDES =

default_target: $(PROJ_NAME)

$(PROJ_NAME): ${OBJS} ${MAIN}
	${CPP} ${CXX_INCLUDES} ${FLAGS} ${OBJS} ${MAIN} -o "$(BUILD_DIR)/$(PROJ_NAME)"
	echo "Built project into $(BUILD_DIR) folder. Run it with ./$(BUILD_DIR)/$(PROJ_NAME)"

tests: ${OBJS} tests.cpp
	${CPP} ${CXX_INCLUDES} ${FLAGS} ${OBJS} tests.cpp -o "${BUILD_DIR}/run_tests"
	echo "Built tests into ${BUILD_DIR} folder. Run them with ./$(BUILD_DIR)/run_tests"

$(BUILD_DIR)/FrequencyAnalyzer.o: kampmeier_aaron_FrequencyAnalyzer.cpp kampmeier_aaron_FrequencyAnalyzer.h
	${CPP} ${CXX_INCLUDES} ${FLAGS} -c kampmeier_aaron_FrequencyAnalyzer.cpp -o $(BUILD_DIR)/FrequencyAnalyzer.o

${BUILD_DIR}/kampmeier_aaron_hw4.o: kampmeier_aaron_hw4.cpp
	${CPP} ${CXX_INCLUDES} ${FLAGS} -c kampmeier_aaron_hw4.cpp -o ${BUILD_DIR}/kampmeier_aaron_hw4.o


clean:
	rm -r $(BUILD_DIR)/*
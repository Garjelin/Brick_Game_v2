CC = g++
FLAG_C = -c
ASAN = -g -fsanitize=address
FLAG_COV = --coverage
FLAG_ER = -Wall -Werror -Wextra -g
CPP_FILES = *.cpp
O_FILES = *.o
LIB_NAME = s21_snake.a
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes
FLAG_TESTS = -lgcov -lsubunit -lgtest -lgtest_main

CONSOLE = snake_consol
DESKTOP = snake_desktop
DESKTOP_TEMP = snake_desktop_temp
SNAKE_MODEL_DIR = brick_game/snake

SNAKE_CONSOLE = snake_console/snake_console.cpp 
SNAKE_MODEL = brick_game/snake/game.cpp 
CONSOLE_CONTROLLER = gui/console/console_controller.cpp 
CONSOLE_VIEW = gui/console/console_view.cpp

BUILD_DIR = BUILD
TEST_DIR = TESTS
SUITE_CASES_CPP = suite_*.cpp
SUITE_CASES_O = suite_*.o

all: install

test: clean s21_snake.a
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	for file in $(TEST_DIR)/$(SUITE_CASES_CPP); do \
		$(CC) $(FLAG_C) $(FLAG_ER) $(FLAG_COV) $$file -o $(BUILD_DIR)/$$(basename $$file .cpp).o; \
	done
	$(CC) $(FLAG_ER) $(FLAG_COV) -o $(BUILD_DIR)/tests $(BUILD_DIR)/$(SUITE_CASES_O) brick_game/snake/game.cpp $(FLAG_TESTS)
	./$(BUILD_DIR)/tests

asan: clean s21_snake.a
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	for file in $(TEST_DIR)/$(SUITE_CASES_CPP); do \
		$(CC) $(FLAG_C) $(FLAG_ER) $(FLAG_COV) $$file -o $(BUILD_DIR)/$$(basename $$file .cpp).o; \
	done
	$(CC) $(FLAG_ER) -o $(BUILD_DIR)/tests $(BUILD_DIR)/$(SUITE_CASES_O) brick_game/snake/game.cpp $(FLAG_TESTS) $(ASAN)
	./$(BUILD_DIR)/tests

valgrind_test: clean s21_snake.a
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	for file in $(TEST_DIR)/$(SUITE_CASES_CPP); do \
		$(CC) $(FLAG_C) $(FLAG_ER) $(FLAG_COV) $$file -o $(BUILD_DIR)/$$(basename $$file .cpp).o; \
	done
	$(CC) $(FLAG_ER) -o $(BUILD_DIR)/tests $(BUILD_DIR)/$(SUITE_CASES_O) brick_game/snake/game.cpp $(FLAG_TESTS)
	valgrind $(VALGRIND_FLAGS) ./$(BUILD_DIR)/tests

gcov_report: test
	gcovr --html-details -o $(BUILD_DIR)/report.html
	xdg-open $(BUILD_DIR)/report.html

lcov_report: test
	lcov -c -d $(BUILD_DIR) -o $(BUILD_DIR)/test_coverage.info

	lcov -r $(BUILD_DIR)/test_coverage.info '/usr/*' -o $(BUILD_DIR)/test_coverage_filtered.info
	lcov -r $(BUILD_DIR)/test_coverage_filtered.info '*/gtest/*' -o $(BUILD_DIR)/test_coverage_filtered.info
	lcov -r $(BUILD_DIR)/test_coverage_filtered.info '*/gtest/internal/*' -o $(BUILD_DIR)/test_coverage_filtered.info
	lcov -r $(BUILD_DIR)/test_coverage_filtered.info '*/bits/*' -o $(BUILD_DIR)/test_coverage_filtered.info
	lcov -r $(BUILD_DIR)/test_coverage_filtered.info '*/ext/*' -o $(BUILD_DIR)/test_coverage_filtered.info
	lcov -r $(BUILD_DIR)/test_coverage_filtered.info '*/suite_*.cpp' -o $(BUILD_DIR)/test_coverage_filtered.info
	genhtml -o $(BUILD_DIR)/report $(BUILD_DIR)/test_coverage_filtered.info
	
	# genhtml -o $(BUILD_DIR)/report $(BUILD_DIR)/test_coverage.info
	xdg-open $(BUILD_DIR)/report/index.html


s21_snake.a:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi
	for file in $(SNAKE_MODEL_DIR)/$(CPP_FILES); do \
        $(CC) $(FLAG_C) $(FLAG_ER) -c -o $(BUILD_DIR)/$$(basename $$file .cpp).o $$file; \
    done
	ar rc $(BUILD_DIR)/$(LIB_NAME) $(BUILD_DIR)/$(O_FILES)
	ranlib $(BUILD_DIR)/$(LIB_NAME)

install: s21_snake.a
	cp -r $(DESKTOP) $(DESKTOP_TEMP)
	qmake -o $(DESKTOP_TEMP)/Makefile $(DESKTOP_TEMP)/snake.pro
	cd $(DESKTOP_TEMP)/; make
	cd ../
	cp $(DESKTOP_TEMP)/snake ./
	rm -rf $(DESKTOP_TEMP)
	g++ $(FLAG_ER) $(SNAKE_CONSOLE) $(SNAKE_MODEL) $(CONSOLE_CONTROLLER) $(CONSOLE_VIEW) -o snake_con -lncurses

run_desk:
	./snake

run_con:
	./snake_con

uninstall:
	rm -rf snake
	rm -rf snake_con
	rm -rf record.txt

# dist:
# 	tar -czf SmartCalc.install.tar.gz ./*

# dvi:
# 	open ./instruction.md

# cpp_check:
# 	cppcheck --enable=all --force $(BACKDIR)/*.h $(BACKDIR)/*.c

# style_check:
# 	cp ../materials/linters/.clang-format ./
# 	clang-format -n $(BACKDIR)/*.c
# 	clang-format -i $(BACKDIR)/*.c
# 	rm -rf .clang-format

clean: uninstall
	-rm -rf $(BUILD_DIR)


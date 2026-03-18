CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra

SRCDIR = ../src
OBJDIR = ../obj
BINDIR = ../bin
INSTALLDIR = ../usr

LDFLAGS = -lncurses -lm
SANITIZE = -fsanitize=address
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
TEST_LIBS = -lcheck -lm -lpthread -lsubunit

TAR = tar
TARFLAGS = -czf
DISTNAME = brick_game_tetris
DISTDIR = ../dist

LIB_NAME = libtetris.so
LIB_SOURCES = $(wildcard $(SRCDIR)/brick_game/tetris/*.c)
LIB_HEAD = $(wildcard $(SRCDIR)/brick_game/tetris/*.h)
LIB_OBJECTS = $(LIB_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = tetris
SOURCES = $(wildcard $(SRCDIR)/gui/cli/*.c)
HEAD = $(wildcard $(SRCDIR)/gui/cli/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

DOXYFILE = Doxyfile
DOCDIR = ../docs

# Основная цель
all: clean $(BINDIR)/$(TARGET)

# Покрытие кода
gcov_report: clean $(BINDIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) tests/test.c tests/test_tetris.c $(LIB_SOURCES) -o $(BINDIR)/gcov_report $(TEST_LIBS) $(LDFLAGS)
	$(BINDIR)/gcov_report
	lcov -c -d $(BINDIR) -o $(BINDIR)/coverage.info
	genhtml -o $(BINDIR)/report $(BINDIR)/coverage.info
	google-chrome $(BINDIR)/report/index.html

# Проверка памяти
sanitize: clean $(BINDIR)
	$(CC) $(CFLAGS) $(SANITIZE) tests/test.c tests/test_tetris.c $(LIB_SOURCES) -o $(BINDIR)/sanitize $(TEST_LIBS) $(LDFLAGS)
	$(BINDIR)/sanitize

# Проверка памяти
valgrind: clean test
	valgrind -q --leak-check=yes --tool=memcheck --show-possibly-lost=no $(BINDIR)/test

# clang-format
cf:
	@find . -type f -name "*.[ch]" -exec clang-format -i {} \;

# Очистка всего
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(DISTDIR) $(DOCDIR)

# Очистка только документации
clean-doc:
	rm -rf $(DOCDIR)

# Создание динамической библиотеки
$(LIB_NAME): $(LIB_OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^ -shared -fPIC

# Создание исполняемого файла
$(BINDIR)/$(TARGET): $(OBJECTS) $(LIB_NAME)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS) -L$(BINDIR) -l$(TARGET) -Wl,-rpath,'$$ORIGIN'

# Компиляция объектных файлов
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ $(LDFLAGS)
# 	$(CC) $(CFLAGS) -fvisibility=hidden -fPIC -c $< -o $@ $(LDFLAGS)

# Создание папок
$(DISTDIR):
	mkdir -p $(DISTDIR)

# Создание папок
$(BINDIR):
	mkdir -p $(BINDIR)

# Создание папок
$(OBJDIR):
	mkdir -p $(OBJDIR)/brick_game/tetris $(OBJDIR)/gui/cli

test: clean $(LIB_NAME) $(BINDIR)
	$(CC) $(CFLAGS) tests/test.c tests/test_tetris.c -o $(BINDIR)/test $(TEST_LIBS) -L$(BINDIR) -ltetris -Wl,-rpath,'$$ORIGIN'

# Установка программы
install: $(BINDIR)/$(TARGET)
	install -d $(INSTALLDIR)
	install -m 755 $(BINDIR)/$(TARGET) $(INSTALLDIR)/$(TARGET)
	install -m 755 $(BINDIR)/$(LIB_NAME) $(INSTALLDIR)/$(LIB_NAME)
	@echo "Программа установлена в $(INSTALLDIR)"

# Удаление установленной программы
uninstall:
	rm -fr $(INSTALLDIR)
	@echo "Программа удалена из $(INSTALLDIR)"

# Создание дистрибутива
dist: $(DISTDIR)
	mkdir -p $(DISTDIR)/$(DISTNAME)/src/gui/cli
	mkdir -p $(DISTDIR)/$(DISTNAME)/src/brick_game/tetris
	cp -r $(SRCDIR)/gui/cli $(DISTDIR)/$(DISTNAME)/src/gui/
	cp -r $(SRCDIR)/brick_game/tetris $(DISTDIR)/$(DISTNAME)/src/brick_game/
	cp -r $(SRCDIR)/Makefile $(DISTDIR)/$(DISTNAME)/src
	cp -r $(SRCDIR)/Doxyfile $(DISTDIR)/$(DISTNAME)/src
	$(TAR) $(TARFLAGS) $(DISTDIR)/$(DISTNAME).tar.gz $(DISTDIR)/$(DISTNAME)

# Цель для генерации DVI документации через Doxygen
dvi: $(DOXYFILE)
	doxygen $(DOXYFILE)
	@if [ -d "$(DOCDIR)/latex" ]; then \
		cd $(DOCDIR)/latex && make && cd -; \
		echo "DVI документация сгенерирована в $(DOCDIR)/latex/refman.dvi"; \
	else \
		echo "Ошибка: LaTeX директория не найдена"; \
	fi

# Создание Doxyfile, если его нет
$(DOXYFILE):
	doxygen -g $(DOXYFILE)
	@echo "Создан шаблонный $(DOXYFILE), пожалуйста, настройте его"

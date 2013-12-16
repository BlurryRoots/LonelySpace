#
# main make file
#

PROJECT_NAME = LudumDare28

BUILDMODE = debug

COMPILER = g++
FLAGS = -std=c++11 -Wall

BINARIES_PATH = bin
OBJECTS_PATH = obj
SOURCE_PATH = src
TEST_PATH = test

ERROR_OUTPUT = 2> error.log

COMPILECOMMAND = $(COMPILER) $(FLAGS)
BUILDCOMMAND = $(COMPILER)	

YANECOS_INCLUDES = -I$(SOURCE_PATH)
SFML_INCLUDES = -Ilibs/Sfml-2.1/include

OUTFILE = $(PROJECT_NAME)

ifeq ($(OS),Windows_NT)
	COPY = copy
	CHMOD =
	SFML_LIBS_PATH = -Llibs/Sfml-2.1/lib/
	SYSTEM_LIBS =
	SFML_BINARIES_PATH = libs/Sfml-2.1/bin
else
	COPY = cp -av
	CHMOD = chmod +x $(BINARIES)/$(OUTFILE)
	SFML_LIBS_PATH = -Llibs/Sfml-2.1/lib/linux
	SYSTEM_LIBS = -lpthread -ldl -lGLEW
	SFML_BINARIES_PATH = libs/Sfml-2.1/lib/linux
endif

SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

#assets and scripts
SCRIPTS_PATH = scripts
ASSETS_PATH = assets

.PHONY: clean-bin clean-obj clean prepare-bin-folder prepare-obj-folders prepare

clean-bin:
	rm -rf $(BINARIES_PATH)

clean-obj:	
	rm -rf $(OBJECTS_PATH)

clean: clean-bin clean-obj

prepare-bin-folder:
	mkdir $(BINARIES_PATH)
	cd $(BINARIES_PATH) && mkdir $(BUILDMODE) && cd ..

prepare-obj-folders:
	mkdir $(OBJECTS_PATH)
	cd $(OBJECTS_PATH) && mkdir $(BUILDMODE) && cd ..

prepare: prepare-obj-folders prepare-bin-folder

build-blurryroots-sfml:
	$(COMPILECOMMAND) $(SFML_INCLUDES) -c $(SOURCE_PATH)/BlurryRoots/Sfml/SfmlGame.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/SfmlGame.o $(ERROR_OUTPUT)

build-blurryroots-yanecos:
	$(COMPILECOMMAND) $(SFML_INCLUDES) -c $(SOURCE_PATH)/BlurryRoots/Yanecos/Database.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/Database.o $(ERROR_OUTPUT)
	$(COMPILECOMMAND) $(SFML_INCLUDES) -c $(SOURCE_PATH)/BlurryRoots/Yanecos/EntityManager.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/EntityManager.o $(ERROR_OUTPUT)
	$(COMPILECOMMAND) $(SFML_INCLUDES) -c $(SOURCE_PATH)/BlurryRoots/Yanecos/ProcessManager.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/ProcessManager.o $(ERROR_OUTPUT)
	$(COMPILECOMMAND) $(SFML_INCLUDES) -c $(SOURCE_PATH)/BlurryRoots/Yanecos/EventManager.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/EventManager.o $(ERROR_OUTPUT)

build-blurryroots: build-blurryroots-sfml build-blurryroots-yanecos
	@echo building BlurryRoots

build-game:
	$(COMPILECOMMAND) $(SFML_INCLUDES) $(YANECOS_INCLUDES) -c $(SOURCE_PATH)/Game.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/Game.o $(ERROR_OUTPUT)

build-entrypoint:
	$(COMPILECOMMAND) $(SFML_INCLUDES) $(YANECOS_INCLUDES) -c $(SOURCE_PATH)/EntryPoint.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/EntryPoint.o $(ERROR_OUTPUT)

build-all: build-blurryroots build-game build-entrypoint

link-executable:
	$(BUILDCOMMAND) $(wildcard $(OBJECTS_PATH)/$(BUILDMODE)/*.o) $(SFML_LIBS_PATH) $(SFML_LIBS) $(SYSTEM_LIBS) -o $(BINARIES_PATH)/$(BUILDMODE)/$(OUTFILE) $(ERROR_OUTPUT)

copy-dynamic-libraries:
	cp -av $(SFML_BINARIES_PATH)/* $(BINARIES_PATH)/$(BUILDMODE) $(ERROR_OUTPUT)

copy-dependencies:
	cp -av $(SCRIPTS_PATH) $(BINARIES_PATH)/$(BUILDMODE)
	cp -av $(ASSETS_PATH) $(BINARIES_PATH)/$(BUILDMODE)

all: clean prepare build-all link-executable copy-dynamic-libraries copy-dependencies

run:
	@echo "-------------------------------------------------------"
	@echo "Running game"
	@$(BINARIES_PATH)/$(BUILDMODE)/./$(OUTFILE)

test-database: clean prepare
	$(COMPILECOMMAND) -c $(SOURCE_PATH)/BlurryRoots/Yanecos/Database.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/Database.o $(ERROR_OUTPUT)
	$(COMPILECOMMAND) -Ilibs/Catch/include -I$(SOURCE_PATH) $(TEST_PATH)/Database.cpp $(OBJECTS_PATH)/$(BUILDMODE)/Database.o -o $(BINARIES_PATH)/$(BUILDMODE)/test-Database $(ERROR_OUTPUT)
	@echo "Running test-Database" && $(BINARIES_PATH)/$(BUILDMODE)/./test-Database

test-entitymanager: clean prepare
	$(COMPILECOMMAND) -c $(SOURCE_PATH)/BlurryRoots/Yanecos/Database.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/Database.o $(ERROR_OUTPUT)
	$(COMPILECOMMAND) -c $(SOURCE_PATH)/BlurryRoots/Yanecos/EntityManager.cpp -o $(OBJECTS_PATH)/$(BUILDMODE)/EntityManager.o $(ERROR_OUTPUT)
	$(COMPILECOMMAND) -Ilibs/Catch/include -I$(SOURCE_PATH) $(TEST_PATH)/EntityManager.cpp $(OBJECTS_PATH)/$(BUILDMODE)/EntityManager.o $(OBJECTS_PATH)/$(BUILDMODE)/Database.o -o $(BINARIES_PATH)/$(BUILDMODE)/test-EntityManager $(ERROR_OUTPUT)
	@echo "Running test-EntityManager" && $(BINARIES_PATH)/$(BUILDMODE)/./test-EntityManager
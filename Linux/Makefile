########################################

##	Makefile for linux version
##	of Void Clash

#######################################

## Path to your libGL.so (path to OpenGL library, which depends on your driver)
## For example, on my computer the library is /usr/lib/nvidia-331/libGL.so.XXX.XX
## so the path is /usr/lib/nvidia-331
PATH_TO_OPENGL_LIBRARY= $(ROOT)/Installer/OpenGL/

## Debug level for graphic engine
DEBUG_FLAGS=		-ggdb3

## Main project folder
ROOT=			$(shell dirname $(PWD))

## Binary names
NAME_CLIENT=		$(ROOT)/Installer/Linux/client
NAME_SERVER=		$(ROOT)/Installer/Linux/server
NAME_OGL=		$(ROOT)/dependencies/liboglGraphic.a

# Commands
RM=			@rm -rf
CC=			@g++-4.8
PRINT=			@printf

## Sources folder
SRCDIR_CLIENT=		$(ROOT)/sources/client/src
SRCDIR_SERVER=		$(ROOT)/sources/server/src
SRCDIR_OGL=		$(ROOT)/sources/API

SRCDIR_EVENT=		$(ROOT)/sources/shared/Event/src
SRCDIR_FILES=		$(ROOT)/sources/shared/Files/src
SRCDIR_GAME=		$(ROOT)/sources/shared/GameEngine/src
SRCDIR_GRAPHIC=		$(ROOT)/sources/shared/GraphicEngine/src
SRCDIR_JSON=		$(ROOT)/sources/shared/LibJson/src
SRCDIR_MAP=		$(ROOT)/sources/shared/Map/src
SRCDIR_PHYSIC=		$(ROOT)/sources/shared/PhysicEngine/src
SRCDIR_SOUND=		$(ROOT)/sources/shared/SoundEngine/src
SRCDIR_COMMON=		$(ROOT)/sources/common/src

# Sources files
SRC_CLIENT=		$(shell find $(SRCDIR_CLIENT) -name "*.cpp")
SRC_SERVER=		$(shell find $(SRCDIR_SERVER) -name "*.cpp")
SRC_OGL=		$(shell find $(SRCDIR_OGL) -name "*.cpp")
SRC_SHARED=		$(shell find $(SRCDIR_SHARED) -name "*.cpp")

SRC_EVENT=		$(shell find $(SRCDIR_EVENT) -name "*.cpp")
SRC_FILES=		$(shell find $(SRCDIR_FILES) -name "*.cpp")
SRC_GAME=		$(shell find $(SRCDIR_GAME) -name "*.cpp")
SRC_GRAPHIC=		$(shell find $(SRCDIR_GRAPHIC) -name "*.cpp")
SRC_JSON=		$(shell find $(SRCDIR_JSON) -name "*.cpp")
SRC_MAP=		$(shell find $(SRCDIR_MAP) -name "*.cpp")
SRC_PHYSIC=		$(shell find $(SRCDIR_PHYSIC) -name "*.cpp")
SRC_SOUND=		$(shell find $(SRCDIR_SOUND) -name "*.cpp")
SRC_COMMON=		$(shell find $(SRCDIR_COMMON) -name "*.cpp")

# Obj folder
OBJDIR_CLIENT=		$(ROOT)/Linux/client/obj
OBJDIR_SERVER=		$(ROOT)/Linux/server/obj
OBJDIR_OGL=		$(ROOT)/Linux/oglGraphic/obj
OBJDIR_SHARED=		$(ROOT)/Linux/shared/obj

OBJDIR_EVENT=		$(ROOT)/Linux/shared/Event/obj
OBJDIR_FILES=		$(ROOT)/Linux/shared/Files/obj
OBJDIR_GAME=		$(ROOT)/Linux/shared/GameEngine/obj
OBJDIR_GRAPHIC=		$(ROOT)/Linux/shared/GraphicEngine/obj
OBJDIR_JSON=		$(ROOT)/Linux/shared/LibJson/obj
OBJDIR_MAP=		$(ROOT)/Linux/shared/Map/obj
OBJDIR_PHYSIC=		$(ROOT)/Linux/shared/PhysicEngine/obj
OBJDIR_SOUND=		$(ROOT)/Linux/shared/SoundEngine/obj
OBJDIR_COMMON=		$(ROOT)/Linux/common/obj

# Obj
OBJ_CLIENT_TMP=		$(SRC_CLIENT:.cpp=.o)
OBJ_CLIENT=		$(subst $(SRCDIR_CLIENT), $(OBJDIR_CLIENT), $(OBJ_CLIENT_TMP))
OBJ_SERVER_TMP=		$(SRC_SERVER:.cpp=.o)
OBJ_SERVER=		$(subst $(SRCDIR_SERVER), $(OBJDIR_SERVER), $(OBJ_SERVER_TMP))
OBJ_OGL_TMP=		$(SRC_OGL:.cpp=.o)
OBJ_OGL=		$(subst $(SRCDIR_OGL), $(OBJDIR_OGL), $(OBJ_OGL_TMP))
OBJ_SHARED=		$(SRC_SHARED:.cpp=.o)
OBJ_SHARED=		$(subst $(SRCDIR_SHARED), $(OBJDIR_SHARED), $(OBJ_SHARED_TMP))

OBJ_EVENT_TMP=		$(SRC_EVENT:.cpp=.o)
OBJ_EVENT=		$(subst $(SRCDIR_EVENT), $(OBJDIR_EVENT), $(OBJ_EVENT_TMP))
OBJ_FILES_TMP=		$(SRC_FILES:.cpp=.o)
OBJ_FILES=		$(subst $(SRCDIR_FILES), $(OBJDIR_FILES), $(OBJ_FILES_TMP))
OBJ_GAME_TMP=		$(SRC_GAME:.cpp=.o)
OBJ_GAME=		$(subst $(SRCDIR_GAME), $(OBJDIR_GAME), $(OBJ_GAME_TMP))
OBJ_GRAPHIC_TMP=	$(SRC_GRAPHIC:.cpp=.o)
OBJ_GRAPHIC=		$(subst $(SRCDIR_GRAPHIC), $(OBJDIR_GRAPHIC), $(OBJ_GRAPHIC_TMP))
OBJ_JSON_TMP=		$(SRC_JSON:.cpp=.o)
OBJ_JSON=		$(subst $(SRCDIR_JSON), $(OBJDIR_JSON), $(OBJ_JSON_TMP))
OBJ_MAP_TMP=		$(SRC_MAP:.cpp=.o)
OBJ_MAP=		$(subst $(SRCDIR_MAP), $(OBJDIR_MAP), $(OBJ_MAP_TMP))
OBJ_PHYSIC_TMP=		$(SRC_PHYSIC:.cpp=.o)
OBJ_PHYSIC=		$(subst $(SRCDIR_PHYSIC), $(OBJDIR_PHYSIC), $(OBJ_PHYSIC_TMP))
OBJ_SOUND_TMP=		$(SRC_SOUND:.cpp=.o)
OBJ_SOUND=		$(subst $(SRCDIR_SOUND), $(OBJDIR_SOUND), $(OBJ_SOUND_TMP))
OBJ_COMMON_TMP=		$(SRC_COMMON:.cpp=.o)
OBJ_COMMON=		$(subst $(SRCDIR_COMMON), $(OBJDIR_COMMON), $(OBJ_COMMON_TMP))

OBJ_SHARED=		$(OBJ_EVENT) $(OBJ_FILES) $(OBJ_GAME) $(OBJ_GRAPHIC) $(OBJ_JSON) $(OBJ_MAP) $(OBJ_PHYSIC) $(OBJ_SOUND) $(OBJ_COMMON)
OBJ_SHARED_SERVER=	$(OBJ_EVENT) $(OBJ_FILES) $(OBJ_GAME) $(OBJ_JSON) $(OBJ_MAP) $(OBJ_PHYSIC) $(OBJ_COMMON)

# Dependencies
DEPS_CLIENT := $(OBJ_CLIENT:.o=.d)
DEPS_SERVER := $(OBJ_SERVER:.o=.d)
DEPS_OGL := $(OBJ_OGL:.o=.d)
DEPS_EVENT := $(OBJ_EVENT:.o=.d)
DEPS_JSON := $(OBJ_JSON:.o=.d)
DEPS_MAP := $(OBJ_MAP:.o=.d)
DEPS_GRAPHIC := $(OBJ_GRAPHIC:.o=.d)
DEPS_GAME := $(OBJ_GAME:.o=.d)
DEPS_PHYSIC := $(OBJ_PHYSIC:.o=.d)
DEPS_SOUND := $(OBJ_SOUND:.o=.d)
DEPS_COMMON := $(OBJ_COMMON:.o=.d)
DEPS_FILES := $(OBJ_FILES:.o=.d)

# Include folder
INCDIR_OGL=		-I$(ROOT)/sources/API \
			-I$(ROOT)/dependencies/Freetype/inc \
			-I$(ROOT)/sources/shared/Files/inc \
			-I$(ROOT)/sources/shared/GraphicEngine/inc \
			-I$(ROOT)/sources/common/inc

INCDIR_SHARED=		-I$(ROOT)/sources/shared/Event/inc \
			-I$(ROOT)/sources/shared/Files/inc \
			-I$(ROOT)/sources/shared/GameEngine/inc \
			-I$(ROOT)/sources/shared/GraphicEngine/inc \
			-I$(ROOT)/sources/shared/LibJson/inc \
			-I$(ROOT)/sources/shared/Map/inc \
			-I$(ROOT)/sources/shared/PhysicEngine/inc \
			-I$(ROOT)/sources/shared/SoundEngine/inc \
			-I$(ROOT)/sources/common/inc/
INCDIR_DEPS=		-I$(ROOT)/dependencies/ \
			-I$(ROOT)/dependencies/SFML/inc \
			-I$(ROOT)/dependencies/glm \
			-I$(ROOT)/dependencies/Tinyxml \
			-I$(ROOT)/dependencies/Assimp/inc \
			-I$(ROOT)/dependencies/GLFW/ \
			-I$(ROOT)/dependencies/cegui-0.8.4/cegui-0.8.4/cegui/include \
			-I$(ROOT)/dependencies/SOIL/inc
INCDIR_CLIENT=		-I$(ROOT)/sources/client/inc $(INCDIR_OGL) $(INCDIR_SHARED) $(INCDIR_DEPS)
INCDIR_SERVER=		-I$(ROOT)/sources/server/inc $(INCDIR_OGL) $(INCDIR_SHARED) $(INCDIR_DEPS)


# Compilation flags
PREPROCESSOR=
CXXFLAGS=		-std=c++11 -W -Wall -Wextra -fpermissive $(PREPROCESSOR) $(DEBUG_FLAGS)

# Link flags
LIB_DIR=		-L$(ROOT)/Installer/Linux -L$(ROOT)/dependencies/ -L$(ROOT)/dependencies/SOIL/lib

LDFLAGS_CLIENT=		-Wl,-rpath=$(ROOT)/Installer/Linux $(LIB_DIR) \
			-loglGraphic -lSOIL \
			-lGL -lGLEW -lglfw \
			-lsfml-audio -lsfml-network -lsfml-system -lsfml-window \
			-lfreetype -ltinyxml -lassimp \
			-lCEGUIBase-0 -lCEGUIOpenGLRenderer-0

LDFLAGS_SERVER=		-Wl,-rpath=$(ROOT)/Installer/Linux $(LIB_DIR) -lsfml-network -lsfml-system

# Main rule
all:	oglGraphic client server

# Client rules
client:			$(NAME_CLIENT)

$(NAME_CLIENT):		$(NAME_OGL) $(OBJ_SHARED) $(OBJ_CLIENT)
			$(CC) $(OBJ_SHARED) $(OBJ_CLIENT) -o $(NAME_CLIENT) $(LDFLAGS_CLIENT)
			$(PRINT) "\033[31;01m==== Client compilation done ! ====\033[00m\n"

$(OBJDIR_CLIENT)%.o:	$(SRCDIR_CLIENT)%.cpp
			$(PRINT) "\033[32;01mClient : Compiling \033[00m\033[35;01m$(notdir $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_SHARED) $(INCDIR_CLIENT) $(INCDIR_DEPS)

-include $(DEPS_CLIENT)

# Server rules
server:			$(NAME_SERVER)

$(NAME_SERVER):		$(OBJ_SHARED_SERVER) $(OBJ_SERVER)
			$(CC) $(OBJ_SHARED_SERVER) $(OBJ_SERVER) -o $(NAME_SERVER) $(LDFLAGS_SERVER)
			$(PRINT) "\033[31;01m==== Server compilation done ! ====\033[00m\n"

$(OBJDIR_SERVER)%.o:	$(SRCDIR_SERVER)%.cpp
			$(PRINT) "\033[32;01mClient : Compiling \033[00m\033[35;01m$(notdir $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_SHARED) $(INCDIR_SERVER) $(INCDIR_DEPS)

-include $(DEPS_SERVER)

# OGL rules
oglGraphic:		$(NAME_OGL)

$(NAME_OGL):		$(OBJ_OGL)
			@ar rcs $(NAME_OGL) $(OBJ_OGL)
			$(PRINT) "\033[31;01m==== OglGraphic compilation done ! ====\033[00m\n"

$(OBJDIR_OGL)%.o:	$(SRCDIR_OGL)%.cpp
			$(PRINT) "\033[32;01mOglGraphic : Compiling \033[00m\033[35;01m$(notdir $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_OGL) $(INCDIR_DEPS)

-include $(DEPS_OGL)

## Shared rules
$(OBJDIR_EVENT)%.o:	$(SRCDIR_EVENT)%.cpp
			$(PRINT) "\033[32;01mEvent : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/shared/Event/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_EVENT)

$(OBJDIR_FILES)%.o:	$(SRCDIR_FILES)%.cpp
			$(PRINT) "\033[32;01mFiles : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/shared/Files/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_FILES)

$(OBJDIR_GAME)%.o:	$(SRCDIR_GAME)%.cpp
			$(PRINT) "\033[32;01mGameEngine : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/shared/GameEngine/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_GAME)

$(OBJDIR_GRAPHIC)%.o:	$(SRCDIR_GRAPHIC)%.cpp
			$(PRINT) "\033[32;01mGraphicEngine : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/shared/GraphicEngine/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_GRAPHIC)

$(OBJDIR_JSON)%.o:	$(SRCDIR_JSON)%.cpp
			$(PRINT) "\033[32;01mLibJson : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/shared/LibJson/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_JSON)

$(OBJDIR_MAP)%.o:	$(SRCDIR_MAP)%.cpp
			$(PRINT) "\033[32;01mMap : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/shared/Map/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_MAP)

$(OBJDIR_PHYSIC)%.o:	$(SRCDIR_PHYSIC)%.cpp
			$(PRINT) "\033[32;01mPhysicEngine : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/shared/PhysicEngine/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_PHYSIC)

$(OBJDIR_SOUND)%.o:	$(SRCDIR_SOUND)%.cpp
			$(PRINT) "\033[32;01mSoundEngine : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/shared/SoundEngine/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_SOUND)

$(OBJDIR_COMMON)%.o:	$(SRCDIR_COMMON)%.cpp
			$(PRINT) "\033[32;01mCommon : Compiling\033[00m\033[35;01m$(subst $(ROOT)/sources/common/src/,, $<)\033[00m\n"
			$(CC) $(CXXFLAGS) -c $< -o $@ $(INCDIR_CLIENT)

-include $(DEPS_COMMON)

## Clean
clean:
	@find $(ROOT)/Linux -name "*.o" -delete
	@find $(ROOT)/Linux -name "*.d" -delete
	@find $(ROOT) -name "*~" -delete
	@find $(ROOT) -name "#*#" -delete
	@find $(ROOT) -name ".#*#" -delete

fclean: clean
	$(RM) $(NAME_CLIENT) $(NAME_SERVER) $(NAME_OGL)

re:	fclean all

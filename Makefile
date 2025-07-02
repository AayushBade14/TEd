#compiler
CXX = g++

#libs
LIBS = -lglfw -lGL

#dirs
INCLUDE = ./Include/ThirdParty
BUILD = ./Build

#flags
CXXFLAGS = -Wall -g -I$(INCLUDE)

#objs
OBJS = ./main.o \
			 ./Include/Shader/Shader.o \
			 ./Include/Texture/Texture.o \
			 ./Include/Memory/VBO/VBO.o \
			 ./Include/Memory/VAO/VAO.o \
			 ./Include/ThirdParty/StbImage/stb_image.o \
			 ./Include/Data/Data.o \
			 ./Include/Collision/Collision.o \
			 ./Include/UI/Button/Button.o \
			 ./Include/Camera/Camera2D.o \
			 ./Include/TextureAtlas/TextureAtlas.o \
			 ./Include/ThirdParty/glad/glad.o

#target
TARGET = $(BUILD)/TEd

#all-rule
all: $(TARGET)

#linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS) $(CXXFLAGS)

#compile-rule
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

#clean-rule
.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)

GLSLC=/usr/local/bin/glslc

#CFLAGS = -std=c++17 -O2
#LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

CFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

# create list of all spv files and set as dependency
vertSources = $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
fragSources = $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = a.out
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): src/*cpp src/*hpp
	mkdir -p build
	g++ $(CFLAGS) -o build/$(TARGET) src/*.cpp $(LDFLAGS)

# make shaders targets
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: a.out
	./build/a.out

clean:
	rm -f build/VulkanTest
	rm -f *.spv
PKG_CONFIG_PATH = $(HPX_LOCATION)/lib/pkgconfig
export PKG_CONFIG_PATH	
MY_HPX_LIBS = $(HOME)/my_hpx_lib
# Prepare the environment so that we can run the command
LD_LIBRARY_PATH = $(MY_HPX_LIBS)
export LD_LIBRARY_PATH

all : run

# Compile the library
libchunk_vector.so : chunk_vector_component.cpp
	c++ -o libchunk_vector.so chunk_vector_component.cpp `pkg-config --cflags --libs hpx_component` -liostreams -DHPX_COMPONENT_NAME=chunk_vector

# Move the library to the library directory
$(MY_HPX_LIBS)/libchunk_vector.so: libchunk_vector.so
	mkdir -p $(MY_HPX_LIBS)
	cp libchunk_vector.so $(MY_HPX_LIBS)

# Create the hpx.ini file
$(HOME)/.hpx.ini:
	echo "[hpx]" > $(HOME)/.hpx.ini
	echo "ini_path = \$$[hpx.ini_default_path]:$(MY_HPX_LIBS)" >> $(HOME)/.hpx.ini

# Create the ini file
$(MY_HPX_LIBS)/chunk_vector.ini:
	echo "$(MY_HPX_LIBS)/chunk_vector.ini" > $(MY_HPX_LIBS)/chunk_vector.ini
	echo "[hpx.components.chunk_vector]" >> $(MY_HPX_LIBS)/chunk_vector.ini
	echo "name = chunk_vector" >> $(MY_HPX_LIBS)/chunk_vector.ini
	echo "path = $(MY_HPX_LIBS)" >> $(MY_HPX_LIBS)/chunk_vector.ini

# Compile the client
test_chunk_vector: test_chunk_vector.cpp $(MY_HPX_LIBS)/libchunk_vector.so
	c++ -o test_chunk_vector test_chunk_vector.cpp `pkg-config --cflags --libs hpx_application` -liostreams -lchunk_vector -L $(MY_HPX_LIBS)

# Run the client, first add our directory to the LD_LIBRARY_PATH
run: test_chunk_vector $(HOME)/.hpx.ini $(MY_HPX_LIBS)/chunk_vector.ini
	./test_chunk_vector

clean:
	rm -f test_chunk_vector libchunk_vector.so

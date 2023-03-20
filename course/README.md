# Sample app

## Building the first time

If you haven't built your docker image, just visit the [docker readme](../docker/docker.md).

To run the application, taking `{REPO_PATH}` as the base repository path and
after building, run the following:


```bash
cd {REPO_PATH}/course
mkdir build
cd build
cmake ..
make
./cpp_course
```

## Project organization

You'll find the following project organization:

- course: source code.
  - include: header files.
  - src: source files.
  - test: testing files.
- docker: docker related files.
  - cpp_course: contains Dockerfiles for development purposes.

### To change the library name

Just go to `{REPO_PATH}/CMakeLists.txt` and replace, in `add_library` macro,
`foo` by your `library_name`.


### To add new source files

Just go to `{REPO_PATH}/CMakeLists.txt` and add, under `LIBRARY_SOURCES`, your
new file.

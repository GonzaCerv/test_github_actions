#  C++ L1 Challenge

C++ Isometry Library Challenge Source repository

## CI Status

[//]: # ('../../' removes '/blob/<branch>/' inserted by GitHub when using relative URLs)
[//]: # (This will brake in branches that include a forward slash in their name)
[![C++ CI](../../actions/workflows/ci.yml/badge.svg)](../../actions/workflows/ci.yml)

## Description

This project contains the basic set of files needed to kick-start the C++ Isometry Library challenge for C++ L1 padawans.

The folder contains both the actual project and a development container.

The goal of this challenge is to write a small library that enables us to determine transformations between coordinate frames in a 3D space, by defining vectors, rotation matrices and isometries to perform the transformations using regular algebraic expressions.

The development container provides a development environment that has all the tools needed for the build process pre-installed. Using this container is optional but recommended, since it provides all the tools needed to build and process the project, with the correct versions needed for it.

## Project organization

The current folder has the following subfolders.

- challenge_01_isometries: Source code of the challenge.
  - include: Header files.
  - src: Source files.
  - test: Testing files.
    - src: Test files to check the generated code.
- challenge_02_matrices: Alternative challenge (WIP - feel free to ignore).
- docker: docker related files.

## Problem statement

Write a small library to work with isometries. Those transformations should be able to correctly express:

- 3D composed translations and rotations.
  - From only translation vectors.
  - From only (Euler) rotation angles.
  - From composed translation vectors and rotation angles.
  - From pre-made useful constants, such as unit vector translation in x axis, etc.
- Composition of transformations.
  - Direct of a 3D coordinate in space A to space B.
  - Inverse of a 3D coordinate in space B to space A.
- IO streaming for ease of use and debugging.
- double support. → There is no need for any other data type.

The interface required for your library is defined through the test files that you will find in the test/src folder.

You will need to develop the following code abstractions to satisfy the tests:

- Vector3: Three element vector, to designate an (x, y, z) coordinate in a coordinate frame.
- Matrix3: 3x3 elements matrix, to write [rotation matrices](https://en.wikipedia.org/wiki/Rotation_matrix).
- Isometry. A [homogeneous-matrix](https://www.brainvoyager.com/bv/doc/UsersGuide/CoordsAndTransforms/SpatialTransformationMatrices.html) abstraction.

We encourage you to consider using the following namespaces:

- ekumen::math
- ekumen::math::test

### Bibliography

- [Planning algorithms, Steven LaValle, Cambridge University. Chapter 3](http://lavalle.pl/planning/ch3.pdf).
- [Homogeneous Transformations notes](https://faculty.sites.iastate.edu/jia/files/inline-files/homogeneous-transform.pdf).


### Requirements

- Build an isometry transformations matrix from:
  - Translation vector.
  - Euler rotation angles.
  - Translation vector and Euler angles.
- Compose transformations in mathematical and semantic fashion.
- Let T be a transformation from space A to space B, and M be the transformation from space B to space A. Provide a mathematical and semantic way to obtain M from T.
- Given a point P in space A, transform it to space B with the isometry T (A → B).
- Given a point Q in space B, transform it to space A with the isometry T (A → B).
- Obtain the translation and rotation (matrix) parts from an isometry.
- Serialize to std::string a vector, a matrix and the isometry.
- Compare equality (to the type epsilon extent) of a vector, matrix and an isometry.
- Satisfy the provided unit tests of the above features.


## Installation

- Install [docker](https://docs.docker.com/engine/install/ubuntu/)

- Build the docker image whose name is `cpp_course`:

  ```sh
  ./docker/build.sh
  ```

- Run a docker container from `cpp_course` called `cpp_course_container`:

  ```sh
  ./docker/run.sh
  ```

And a prompt in the docker image should appear at the root of the workspace.

## Usage

To build and test your code:

```bash
cd {REPO_PATH}/challenge_01_isometries
mkdir build
cd build
cmake ..
make
ctest
```

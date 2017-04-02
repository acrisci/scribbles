# libcatfacts

A C++ library that gives you cat facts from the [cat facts api](https://catfacts-api.appspot.com/doc.html).

## Source info

The `libcatfacts` folder contains the library and the `app` folder contains the reference impelementation.

## Building

```bash
mkdir build
cd build
cmake ..
make
```

This produces libcatfacts.a which is a library that gives you a `CatFacts` class with a method `get()` which gets a cat fact.

## Running The Reference Implementation

```bash
./build/app/catfactscli
```

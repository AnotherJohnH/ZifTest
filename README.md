# ZifTest

A set of regression tests for Zif. Zif is a Z-code engine for playing interactive fiction games.
See [Zif](https://github.com/AnotherJohnH/Zif/)

## Status

[![Build Status](https://travis-ci.org/AnotherJohnH/ZifTest.svg?branch=master)](https://travis-ci.org/AnotherJohnH/ZifTest)

## How to run

Run the Python script GO with the --all command line argument to build Zif, download test
story files and run the regression tests. e.g.

```
GO --all
```

The first time this is done the download phase will take around 20 minutes. Subsequent
runs will only download new stories and so will likely take around 2 minutes to complete
all the testing. For other options try...

```
GO --help
```

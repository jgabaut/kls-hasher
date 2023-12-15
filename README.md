# kls-hasher

A toy filter using Koliseo ([link to repo](https://github.com/jgabaut/koliseo)) to hash passed strings.

It's not really useful, just a proof-of-concept.

Check out [main.c](https://github.com/jgabaut/kls-hasher/blob/master/src/main.c).

```console
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C                                2             25             24            161
C/C++ Header                     1             12              2             13
-------------------------------------------------------------------------------
SUM:                             3             37             26            174
-------------------------------------------------------------------------------
```

## Building

To build this repo, you need to install `koliseo`.

  - Clone the repo with:

  ```sh
  git clone git@github.com:jgabaut/koliseo.git
  ```

  - Install `koliseo` with debug build:

  ```
  cd $KOLISEO_REPO || exit 1;
  ./configure --enable-debug=yes && make rebuild && make install
  ```

If you have installed `koliseo` previously, you can build the binary with:

```sh
./scripts/bootstrap_anvil.sh repo_invil && ./anvil build
```

If you don't have a rust compiler, you can build with:
```sh
./scripts/bootstrap_anvil.sh repo_amboso && ./anvil build
```

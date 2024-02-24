# PHP call external code benchmark

This repository contains source codes for benchmarking different ways of calling external code from PHP.

Implemented versions:
- pure PHP
- PHP path with minimal use of functions
- pure PHP with JIT
- pure PHP with preloading
- C version compiled into a library called through FFI
- Rust version compiled into a library called through FFI
- PHP extension in C
- PHP extension in Rust
- PHP extension with optimization in assembler (manual vectorization with SSE2, AVX)

## Reproducing results

Everyone can reproduce results from the article/post/presentation. You will need Docker and Docker compose to get started.

## Starting Docker Container with a prebuilt environment

```bash
docker-compose run cli
```

or (for Compose V2)

```bash
docker compose run cli
```

```bash
./vendor/bin/phpbench run --report=connectors_comparison --output=html --report=aggregate --report=benchmark --report=overview
```


##  Extension

Inside docker:
```bash
make clean
phpize
./configure
make
make test
sudo cp cosine_similarity_c_extension/modules/cosine_similarity_c.so /usr/local/lib/php/extensions/no-debug-non-zts-20220829/
```

## FFI library

Inside docker:
```bash
make clean
make
```

## Rust extension

Inside docker:
```bash
cargo build --release
sudo cp cosine-similarity-rust/target/release/libcosine_similarity.so /usr/local/lib/php/extensions/no-debug-non-zts-20220829/
```

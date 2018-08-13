# yasc
`yasc` is a (budding) interpreter for R6RS scheme. Currently, its operates
exclusively as a scheme repl, but it will eventually support various
interpretation modes, including interpreting files. Implementing these are
lower priority than becoming R6RS compliant, however.

> Warning: `yasc` is in extremely early development, and can only handle nested
> arithmetic (integer) expressions, eg `(+ 1 2 (* 4 5 (- 8 9)))` evaluates to
> `-17`. There is not a warning infrastructure developed yet, so malformed
> queries will result in a segmentation fault.

In this sense, `yasc` is (as of now) only a glorified RPN calculator.

## Usage

Running `yasc` will begin a repl session. Running `yasc-test` will run the test
suite (through `google-test`, so all the same configuration applies to
`yasc-test` as would regular `google-test` projects).

## Building

`yasc` is built through `cmake`, so the following commands should clone and
build the project:

```
git clone https://github.com/jacob-hegna/yasc.git
mkdir -p yasc/build
cd yasc/build
cmake -G Ninja ../
ninja
```

`yasc` currently requires a compiler which supports c++17. I believe the only
actual instances of non-c++14 features are structured bindings, but I do not
have plans to support earlier compilers.

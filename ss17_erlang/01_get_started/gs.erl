-module(gs).
-export([fib/1]).

fib(0) -> 0;
fib(1) -> 1;
fib(N) when N >=  2 -> fib( N - 1) + fib( N - 2);
fib(N) -> io:format("The function fib(~p) is undefined~n", [N]).

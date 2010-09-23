-module(conc).
-export([start/0, proc/2, serv/0]).

%% Simple function using Erlang process

proc(Greeting, 0) ->
  io:format("~p terminate~n", [Greeting]);
proc(Greeting, X) ->
  io:format("~p: ~p~n", [Greeting, X]),
  proc(Greeting, X - 1).

start() ->
  spawn(conc, proc, ["Hello", 5]),
  spawn(conc, proc, ["Hi", 5]).  

%% Messaging

serv() ->
  receive
    Msg -> io:format("echo: ~p~n", [Msg]),
    serv()
  end.
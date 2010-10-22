-module(conc).
-export([start/0, proc/2, serv/0, count_serv/1, counter_serv/1, mstart/2]).
-export([list_to/1, change_at/3, gameloop/4]).

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

%% Counting down

count_serv(Cnt) ->
  receive
    Msg -> io:format("~p~n", [Cnt]),
    count_serv(Cnt - 1)
  end.

%% Counter

counter_serv(Cnt) ->
  receive
    up -> io:format("~p~n", [Cnt]),
                counter_serv(Cnt + 1);
    down -> io:format("~p~n", [Cnt]),
                  counter_serv(Cnt - 1)
  end.

%% Mine Sweeper

mstart( W, N) when (N rem W =/= 0) -> 'Max numbers of bombs must be divide by width of the field';
mstart( W, N) -> 
  Field = gen_field( W, N),
  Mask  = lists:duplicate( W, lists:duplicate( W, c)),
  spawn( conc, gameloop, [W, N, Field, Mask]).

gameloop( W, N, Field, Mask) ->
  receive
    {show} -> show(Field, Mask),
              gameloop( W, N, Field, Mask);
    {open,{X,Y}} -> gameloop( W, N, Field, open({X,Y},Mask))
  end.

show(Field, Mask) ->
  W = list_to( length( Field)),
  ShowRow = fun(List) ->
    lists:foreach( fun(L) ->	
        io:format("~p", [lists:nth( L, List)])
      end,
      W),
    io:format("~n", [])
  end,
  lists:foreach( fun(L) ->
      ShowRow( lists:nth( L, Mask))
    end,
    W).

gen_field( W, N) ->
  Generator = fun(_) -> gen_row( W, trunc( N / W), [], random:uniform(4)) end,
  lists:map( Generator, lists:duplicate( W, dammy)).

gen_row( 0, _, List, _) -> List;
gen_row( W, 0, List, _) -> gen_row( W - 1, 0,     List ++ [n], 2);
gen_row( W, N, List, 1) -> gen_row( W - 1, N - 1, List ++ [b], random:uniform(4));
gen_row( W, N, List, _) -> gen_row( W - 1, N,     List ++ [n], random:uniform(4)).

list_to(0) -> [];
list_to(N) -> lists:append( list_to( N - 1), [N]).

open( {X,Y}, Mask) ->
  change_at( Y, Mask, [change_at( X, lists:nth( Y, Mask), [o])]).

change_at( N, List, E) -> 
  {Car, Cdr} = lists:split( N, List),
  {Pre,_} = lists:split( length(Car) - 1, Car),
  Pre ++ E ++ Cdr.
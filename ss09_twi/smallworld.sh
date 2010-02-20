#!/bin/sh

/usr/bin/ruby ./smallworld.rb $1 $2 > "world_$1_`date +\"%Y%m%d\"`_$2.json"
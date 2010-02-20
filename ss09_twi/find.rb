require 'rubygems'
require 'json'

if ARGV.length < 2
  STDERR.puts "Usage: ruby find.rb world.json screen_name"
  exit
end

name = ARGV[1]
found = false
topic = nil

open(ARGV[0]) do |f|
  topic = []
  any = JSON.parse( f.read)
  any.each do |e|
    if e['name'] == name
      found = true
      topic << e['name']
      break
    end
  end

  if !found
    any.each do |e|
      e['relators'].each do |ee|
        if ee == name
          found = true
          topic = [e['name'], ee]
          break
        end
      end
      break if found
    end  
  end
end

if topic.empty?
  puts "User #{name} not found"
else
  ans = ['*']
  ans << topic
  puts ans.join(' - ')
end

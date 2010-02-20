def following_of(name)
  get_relation(name, 'following')
end
def followers_of(name)
  get_relation(name, 'followers')
end

def get_relation( name, type)
  cookie_path = '/tmp/twitter_client.cookie.yaml'
  agent = Mechanize.new
  
  logger = Logger.new STDERR
  logger.level = Logger::INFO
  agent.log = logger
  
  agent.user_agent_alias = 'Windows IE 7'
  agent.cookie_jar.load cookie_path if File.exist? cookie_path
  
  uri = URI.parse "http://twitter.com/#{name}/#{type}"
  top = agent.get uri
  
  if top.uri.path == '/login'
    username = 'tnzk'
    password = 'c5xtwitter0026'
    f = top.forms[1]
    f['session[username_or_email]'] = username
    f['session[password]'] = password
    #f.checkboxes.name('remember_me').check
    top = f.submit
  end
  
  followings = []
  loop do
    page = agent.page
    
    page.root.search('span.screenname a').each do |element|
      followings << element.inner_html
    end

    next_button = (page / 'a[@rel*="next"]')
    break if next_button.empty?
    begin
      agent.get next_button.attr('href')
    rescue
      STDERR.print('FAULT')
    end
  end
  agent.cookie_jar.save_as cookie_path
  followings
end

require 'rubygems'
require 'json'
require 'mechanize'
require 'logger'

h = {}

def get_relators(target)
  followers = followers_of(target)
  following = following_of(target)
  relators = followers & following
end

def get_zone(h, level = 3)
  # An argument ``h'' is required to have two following sets:
  #       name: a name of target
  #   relators: an array that includes relators of target
  if level == 0
    h
  else
    if h[:relators]
      h[:relators].map do |name|
        hh = {:name => name, :relators => nil}
        hh[:relators] = get_relators(name) if !exist?(h, name)
        get_zone(hh, level - 1)
      end
    end
  end
end

def exist?(h, name)
  return true if h[:name] == name && h[:relators] != nil && h[:relators].class != Array
  return exist?(h, name) if h[:relators].class == Hash
  false
end

if ARGV.length < 2
  STDERR.puts 'Usage: ruby smallworld.rb screen_name depth'
  exit
end

target = ARGV[0]
h = get_zone({:name => target, :relators => get_relators(target)},ARGV[1].to_i)
puts h.to_json




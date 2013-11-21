#!/usr/bin/env ruby

require 'webrick'

httpd = WEBrick::HTTPServer.new(:Port => 8080,
                                :DocumentRoot => Dir::pwd)
Signal.trap('INT') { httpd.shutdown }
httpd.start

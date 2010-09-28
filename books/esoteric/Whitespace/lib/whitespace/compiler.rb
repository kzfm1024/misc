# -*- coding: utf-8 -*-
# codeing: utf-8
require 'strscan'

module Whitespace

  class Compiler

    class ProgramError < StandardError; end

    NUM = /([ \t]+)\n/
    LABEL = NUM

    def self.compile(src)
      new(src).compile
    end

    def initialize(src)
      @src = src
      @s = nil
    end

    def compile
      @s = StringScanner.new(bleach(@src))
      insns = []
      until @s.eos?
        insns.push(step)
      end
      insns
    end

    private

    def bleach(src)
      src.gsub(/[^ \t\n]/, "")
    end

    def step
      case
      when @s.scan(/  #{NUM}/)       then [:push, num(@s[1])]
      when @s.scan(/ \n /)           then [:dup]
      when @s.scan(/ \t #{NUM}/)     then [:copy, num(@s[1])]
      when @s.scan(/ \n\t/)          then [:swap]
      when @s.scan(/ \n\n/)          then [:discard]
      when @s.scan(/ \t\n#{NUM}/)    then [:slide, num(@s[1])]

      when @s.scan(/\t   /)          then [:add]
      when @s.scan(/\t  \t/)         then [:sub]
      when @s.scan(/\t  \n/)         then [:mul]
      when @s.scan(/\t \t /)         then [:div]
      when @s.scan(/\t \t\t/)        then [:mod]

      when @s.scan(/\t\t /)          then [:heap_write]
      when @s.scan(/\t\t\t/)         then [:heap_read]

      when @s.scan(/\n  #{LABEL}/)   then [:label, label(@s[1])]
      when @s.scan(/\n \t#{LABEL}/)  then [:call, label(@s[1])]
      when @s.scan(/\n \n#{LABEL}/)  then [:jump, label(@s[1])]
      when @s.scan(/\n\t #{LABEL}/)  then [:jump_zero, label(@s[1])]
      when @s.scan(/\n\t\t#{LABEL}/) then [:jump_negative, label(@s[1])]

      when @s.scan(/\n\t\n/)         then [:return]
      when @s.scan(/\n\n\n/)         then [:exit]

      when @s.scan(/\t\n  /)         then [:char_out]
      when @s.scan(/\t\n \t/)        then [:num_out]
      when @s.scan(/\t\n\t /)        then [:char_in]
      when @s.scan(/\t\n\t\t/)       then [:num_in]
      else
        raise ProgramError, "どの命令にもマッチしませんでした"
      end
    end

    def num(str)
      if str !~ /\A[ \t]+\z/
        raise ArgumentError, "数値はスペースとタブで指定してください(#{str.inspect})"
      end
      num = str.sub(/\A /, "+").sub(/\A\t/, "-").gsub(/ /, "0").gsub(/\t/, "1")
      num.to_i(2)
    end

    def label(str)
      str
    end

  end

end

if $0 == __FILE__
  p Whitespace::Compiler::compile("   \t\n\t\n \t\n\n\n")
end

require 'whitespace/compiler'
require 'whitespace/vm'

module Whitespace

  def self.run(src)
    insns = Whitespace::Compiler.compile(src)
    Whitespace::VM.run(insns)
  end

end

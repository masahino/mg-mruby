module MG
  def self.add_mode(mode_name, mode_obj)
    if $mg_mode_list == nil
      $mg_mode_list = Hash.new
    end
    $mg_mode_list[mode_name] = mode_obj
    @@mode_list[mode_name] = mode_obj
  end
  def self.get_mode_list
    @@mode_list
  end
  
  class Mode
    attr_accessor :mode_name, :command_list, :keymap
    def initialize(mode_name)
      @mode_name = mode_name
      @command_list = Hash.new
      mode_func = CFunc::Closure.new(CFunc::Int, [CFunc::Int, CFunc::Int]) do |f, n|
        begin 
          MG.message $mg_mode_list[mode_name].mode_name
          MG.change_mode(mode_name)
        rescue e
          puts e
        end
        MG::TRUE
      end
      CFunc::call(CFunc::Int, "funmap_add", mode_func, mode_name+"-mode")
      @command_list[mode_name+"-mode"] = mode_func
      @keymap = MG::Keymap.new
      MG.add_map(@keymap, mode_name)
      MG.add_mode(mode_name, self)
    end

    def add_command(command_name, &block)
#      command_closure = CFunc::Closure.new(CFunc::Int, [CFunc::Pointer(CFunc::Int), CFunc::Pointer(CFunc::Int)]) do |f, n|
      command_closure = CFunc::Closure.new(CFunc::Int, [CFunc::Int, CFunc::Int]) do |f, n|
        begin
          block.call(f, n)
          MG.message "command called"
        rescue => e
          puts e
        end
        MG::TRUE
      end
      CFunc::call(CFunc::Int, "funmap_add", command_closure, command_name)
      @command_list[command_name] = command_closure
    end
  end



end

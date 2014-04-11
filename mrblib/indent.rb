module MG
  def self.indent (n)
    cur_pos = MG.current_position
    cur_string = MG.buffer_string[cur_pos[0]-1]
    space_count = 0
    for i in 0..cur_string.length-1
      if cur_string[i] != " "
        break
      end
      space_count += 1
    end
    if n > space_count
      MG.beginning_of_line
      (n-space_count).times { MG.insert " " }
      if n > cur_pos[1]
        MG.beginning_of_line
        MG.forward_char n
      else
        MG.forward_char cur_pos[1]
      end
    else
      (space_count-n).times { MG.delete_leading_space }
      if n > cur_pos[1]
        MG.beginning_of_line
        MG.forward_char n
      else        
        MG.forward_char cur_pos[1] - (space_count-n)
      end
    end
  end
end

def MG.message(message_str)
  cur_buf = MG::Buffer.current()
  ret = MG.switch_to_buffer("*Messages*")
  if MG::Buffer.current.name != "*Messages*"
    MG.debug_log "failed to change buffer "
  end
  MG.end_of_buffer()
  MG.insert(message_str)
  MG.newline()
  MG.switch_to_buffer(cur_buf.name)
end

MG.message "load message.rb done."

MG.message "file-encodings.rb"
MG.auto_execute("*") do
  file_encoding = "utf-8"
  # detect encoding
  buf_lines = []
  buf_lines.push(MG.buffer_string)
  while MG.next_line
    buf_lines.push(MG.buffer_string)
  end
#  MG.message buf_lines[0]
  MG.message buf_lines.length.to_s
  MG.get_fileencodings.each do |from|
    begin
      ret = Iconv.conv("utf-8", from, buf_lines.join)
    rescue RuntimeError
      next
    end
    file_encoding = from
    break
  end
  if file_encoding != "utf-8"
    MG.beginning_of_buffer
    MG.kill_line buf_lines.length
    buf_lines.each do |line|
      MG.insert Iconv.conv("utf-8", file_encoding, line)
      MG.newline
    end
  end
  MG::Buffer.current.set_encoding(file_encoding)
  MG.message file_encoding
  MG.beginning_of_buffer
end

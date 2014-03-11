MG.message "file-encodings.rb"
MG.add_hook('find-file-hooks') do |buffer|
  MG.message "find-file-hooks called"
  file_encoding = "utf-8"
  # detect encoding
  buf_lines = []
  max_line = MG::Buffer.current.count_lines_page
  MG.message "max_line = " + max_line.to_s
  for i in 0..max_line-1
    buf_lines.push(MG.buffer_string)
    MG.next_line
  end

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
      #       MG.insert Iconv.conv("utf-8", file_encoding, line).gsub("\"", "\\\"")
      MG.insert Iconv.conv("utf-8", file_encoding, line)
      MG.newline
    end
  end
  MG::Buffer.current.set_encoding(file_encoding)
  MG.message file_encoding
  MG.beginning_of_buffer
end

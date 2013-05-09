require 'mruby-nkf'


def encoding_code_to_str(code)
  case code
  when Kconv::JIS
    return "ISO-2022-JP"
  when Kconv::EUC
    return "EUC-JP"
  when Kconv::UTF8
    return "UTF-8"
  end
  return "UTF-8"
end

def encoding_str_to_code(str)
  case str
  when "ISO-2022-JP"
    return Kconv::JIS
  when "EUC-JP"
    return Kconv::EUC
  when "UTF-8"
    return Kconv::UTF8
  end
end

def save_buffer_encode()
  MG.message "save_buffer_encode"
  cur_buf = MG::Buffer.current
  cur_pos = MG.current_posision
  to_enc = encoding_str_to_code(cur_buf.get_encoding)
  src_str = ""
  new_buf = MG::Buffer.new(cur_buf.name + "-save-tmp")
  MG.beginning_of_buffer
  begin
    src_str = MG.buffer_string
    MG.message src_str
    src_str = Kconv.kconv(src_str, to_enc)
    MG.switch_to_buffer new_buf.name
    MG.insert src_str
    MG.newline
    MG.switch_to_buffer cur_buf.name
  end while MG.next_line
end

MG.auto_execute("*") do
  file_encoding = "UTF-8"
  # detect encoding
  l = 1
  while MG.next_line
    src_str = MG.buffer_string
    line_encoding_code = NKF.guess(src_str)
    if line_encoding_code != Kconv::ASCII and line_encoding_code != Kconv::UTF8
      MG.kill_line
      MG.insert Kconv.toutf8(src_str)
#      MG.newline
    end
    if line_encoding_code != Kconv::ASCII and line_encoding_code != Kconv::UTF8
      file_encoding = encoding_code_to_str(line_encoding_code)
    end
  end
  MG::Buffer.current.set_encoding file_encoding
  MG.message "encode = " + file_encoding
end

MG.message("load japanese.rb done")

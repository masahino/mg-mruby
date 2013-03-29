require 'mruby-nkf'
def encoding_code_to_str(code)
  case code
  when Kconv::JIS
    "ISO-2022-JP"
  when Kconv::EUC
    "EUC-JP"
  when Kconv::UTF8
    "UTF-8"
  end
  "UTF-8"
end

MG.auto_execute("*") do
  file_encoding = "ASCII"
  # detect encoding
  l = 1
  while MG.next_line
    src_str = MG.buffer_string
    MG.message "[" + src_str + "]"
    line_encoding_code = NKF.guess(src_str)
    MG.message line_encoding_code.to_s
    if line_encoding_code != Kconv::ASCII and line_encoding_code != Kconv::UTF8
      MG.kill_line
      MG.insert Kconv.toutf8(src_str)
#      MG.newline
    end
    if line_encoding_code != Kconv::ASCII
      file_encoding = encoding_code_to_str(line_encoding_code)
    end
  end
  MG.message "encode = " + file_encoding
end

MG.message("load japanese.rb done")

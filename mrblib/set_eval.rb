cur_buf = MG::Buffer.current
MG.switch_to_buffer("*scratch*")
MG.local_set_key "\\^j", "eval-print-last-exp"
MG.switch_to_buffer(cur_buf.name)


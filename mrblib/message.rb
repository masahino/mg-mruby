def MG.message(message_str)
	cur_buf = MG::Buffer.current()
	MG.switch_to_buffer("*Messages*")
        MG.end_of_buffer()
	MG.insert(message_str)
	MG.newline()
	MG.switch_to_buffer(cur_buf.name)
end

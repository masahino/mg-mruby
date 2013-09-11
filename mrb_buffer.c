/*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "def.h"

#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"

#include "mrb_buffer.h"

static void
mrb_buffer_free(mrb_state *mrb, void *ptr)
{
     struct buffer *bp;
     bp = (struct buffer *)ptr;
     if (bfind(bp->b_bname, FALSE) == NULL) {
	  mrb_free(mrb, ptr);
     }
}

static struct mrb_data_type mrb_buffer_type = { "Buffer", mrb_buffer_free };

mrb_value
mrb_buffer_obj_value(mrb_state *mrb, struct RClass *class, struct buffer *bp)
{
    return mrb_obj_value(Data_Wrap_Struct(mrb, class, &mrb_buffer_type, bp));
}

mrb_value
mrb_buffer_get_name(mrb_state *mrb, mrb_value self)
{
     struct buffer *bp;
     bp = (struct buffer *)mrb_data_get_ptr(mrb, self, &mrb_buffer_type);
     if (bp == NULL)
	  return mrb_nil_value();
     return mrb_str_new_cstr(mrb, strdup(bp->b_bname));
}

mrb_value
mrb_buffer_get_current(mrb_state *mrb, mrb_value self)
{
     if (curbp != NULL) {
	  return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(self),
						&mrb_buffer_type, curbp));
	  return self;
     }
     else {
	  return mrb_nil_value();
     }
}

mrb_value
mrb_buffer_set_encoding(mrb_state *mrb, mrb_value self)
{
     struct buffer *bp;
     char *encode_str;

     bp = (struct buffer *)mrb_data_get_ptr(mrb, self, &mrb_buffer_type);
     if (bp == NULL)
	  return mrb_nil_value();
     mrb_get_args(mrb, "z", &encode_str);
     strncpy(bp->b_encoding, encode_str, NLINE);
     return mrb_true_value();
}

mrb_value
mrb_buffer_get_encoding(mrb_state *mrb, mrb_value self)
{
	struct buffer *bp;
	bp = (struct buffer *)mrb_data_get_ptr(mrb, self, &mrb_buffer_type);
	if (bp == NULL)
		return mrb_nil_value();
	return mrb_str_new_cstr(mrb, bp->b_encoding);
}

mrb_value
mrb_buffer_initialize(mrb_state *mrb, mrb_value self)
{
	mrb_value mrb_buf_name;
	struct buffer *bp;
	char *buf_name;

	bp = (struct buffer *)DATA_PTR(self);
	      
	if (bp) {
		mrb_buffer_free(mrb, bp);
	}
	DATA_TYPE(self) = &mrb_buffer_type;
	DATA_PTR(self) = NULL;
	mrb_get_args(mrb, "S", &mrb_buf_name);
	buf_name = strndup(RSTRING_PTR(mrb_buf_name),
			   RSTRING_LEN(mrb_buf_name));
	bp = bfind(buf_name, TRUE);
	if (bp != NULL) 
		DATA_PTR(self) = bp;
	return self;
}

void
mrb_buffer_init(mrb_state *mrb)
{
    struct RClass *mg, *buffer;

    mg = mrb_class_get(mrb, "MG");
    buffer = mrb_define_class_under(mrb, mg, "Buffer", mrb->object_class);
    MRB_SET_INSTANCE_TT(buffer, MRB_TT_DATA);
    mrb_define_class_method(mrb, buffer, "current", mrb_buffer_get_current,
			    ARGS_NONE());
    mrb_define_method(mrb, buffer, "initialize", mrb_buffer_initialize, 
		       ARGS_OPT(1));
    mrb_define_method(mrb, buffer, "name", mrb_buffer_get_name, ARGS_NONE());

    mrb_define_method(mrb, buffer, "set_encoding", mrb_buffer_set_encoding,
		      ARGS_REQ(1));
    mrb_define_method(mrb, buffer, "get_encoding", mrb_buffer_get_encoding,
			ARGS_NONE());
}

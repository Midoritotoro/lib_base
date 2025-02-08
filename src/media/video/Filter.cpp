#include "Filter.h"


namespace FFmpeg {
    picture_t* FilterNewPicture(filter_t* p_filter) {
        picture_t* pic = NULL;
        if (p_filter->owner.video != NULL && p_filter->owner.video->buffer_new != NULL)
            pic = p_filter->owner.video->buffer_new(p_filter);
        if (pic == NULL)
        {
            // legacy filter owners not setting a default filter_allocator
            pic = PictureNewFromFormat(&p_filter->fmt_out.video);
        }
        if (pic == NULL)
            printf("can't get output picture");
        return pic;
    }
} // namespace FFmpeg
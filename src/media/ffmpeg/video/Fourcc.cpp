#include <base/media/ffmpeg/video/Fourcc.h>


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

int FourccCmp(
    const void* key,
    const void* ent)
{
    return memcmp(key, ent, 4);
}

fourcc_t FourccLookup(
    fourcc_t fourcc,
    const char** dsc,
    const struct fourcc_mapping* mapv, size_t mapc,
    const struct fourcc_desc* dscv, size_t dscc)
{
    const struct fourcc_mapping* mapping;
    const struct fourcc_desc* desc;

    mapping = (fourcc_mapping*)bsearch(&fourcc, mapv, mapc, sizeof(*mapv), FourccCmp);
    if (mapping != NULL)
    {
        if (dsc != NULL)
        {
            desc = (fourcc_desc*)bsearch(&fourcc, dscv, dscc, sizeof(*dscv), FourccCmp);
            if (desc != NULL)
            {
                *dsc = desc->desc;
                return mapping->fourcc;
            }
        }
        fourcc = mapping->fourcc;
    }

    desc = (fourcc_desc*)bsearch(&fourcc, dscv, dscc, sizeof(*dscv), FourccCmp);
    if (desc == NULL)
        return 0; /* Unknown FourCC */
    if (dsc != NULL)
        *dsc = desc->desc;
    return fourcc; /* Known FourCC (has a description) */
}

fourcc_t FourccLookupVideo(
    fourcc_t fourcc,
    const char** dsc)
{
    return FourccLookup(fourcc, dsc, mapping_video, ARRAY_SIZE(mapping_video),
        desc_video, ARRAY_SIZE(desc_video));
}

fourcc_t FourccLookupAudio(
    fourcc_t fourcc,
    const char** dsc)
{
    return FourccLookup(fourcc, dsc, mapping_audio, ARRAY_SIZE(mapping_audio),
        desc_audio, ARRAY_SIZE(desc_audio));
}

fourcc_t FourccLookupSpu(
    fourcc_t fourcc,
    const char** dsc)
{
    return FourccLookup(fourcc, dsc, mapping_spu, ARRAY_SIZE(mapping_spu),
        desc_spu, ARRAY_SIZE(desc_spu));
}

fourcc_t FourccLookupCat(
    fourcc_t fourcc,
    const char** dsc,
    int cat)
{
    switch (cat)
    {
    case VIDEO_ES:
        return FourccLookupVideo(fourcc, dsc);
    case AUDIO_ES:
        return FourccLookupAudio(fourcc, dsc);
    case SPU_ES:
        return FourccLookupSpu(fourcc, dsc);
    }

    fourcc_t ret = FourccLookupVideo(fourcc, dsc);
    if (!ret)
        ret = FourccLookupAudio(fourcc, dsc);
    if (!ret)
        ret = FourccLookupSpu(fourcc, dsc);
    return ret;
}

fourcc_t FourccGetCodec(int cat, fourcc_t fourcc)
{
    fourcc_t codec = FourccLookupCat(fourcc, NULL, cat);
    return codec ? codec : fourcc;
}

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END


#include <base/concurrent/common/CommonAncillary.h>
#include <base/Platform.h>


namespace Threads {
    void AncillaryRelease(ancillary* ancillary) {
        if (AtomicRcDec(&ancillary->rc) == false)
            return;

        if (ancillary->free_cb != NULL)
            ancillary->free_cb(ancillary->data);

        free(ancillary);
    };

    void AncillaryArrayClear(ancillary*** array) {
        if (*array == NULL)
            return;

        for (struct ancillary** ancillary = *array;
            *ancillary != NULL; ancillary++)
            AncillaryRelease(*ancillary);

        free(*array);
        *array = NULL;
    }

    void AncillaryArrayInit(ancillary*** array) {
        *array = NULL;
    }


    ancillary* AncillaryHold(ancillary* ancillary)
    {
        AtomicRcInc(&ancillary->rc);
        return ancillary;
    }

    void* AncillaryGetData(const ancillary* ancillary)
    {
        return ancillary->data;
    }

    size_t AncillaryArrayCount(ancillary** array)
    {
        size_t count = 0;
        for (ancillary** ancillary = array;
            *ancillary != NULL; ancillary++)
        {
            count++;
        }

        return count;
    }

    int AncillaryArrayDup(ancillary*** dst_arrayp,
            ancillary** const* src_arrayp)
    {
        if (unlikely(*dst_arrayp != NULL))
            AncillaryArrayClear(dst_arrayp);

        if (*src_arrayp == NULL)
            return SUCCESS;

        ancillary** src_array = *src_arrayp;
        size_t count = AncillaryArrayCount(src_array);

        ancillary** dst_array =
            (ancillary**)MemoryAllocate(count + 1, sizeof(ancillary*));

        if (dst_array == NULL)
            return ENOMEM;

        for (size_t i = 0; i < count; ++i)
        {
            dst_array[i] = AncillaryHold(src_array[i]);
            assert(dst_array[i] != NULL);
        }
        dst_array[count] = NULL;
        *dst_arrayp = dst_array;

        return SUCCESS;
    }
} // namespace Threads
#pragma once 

#include <base/concurrent/win/WinapiThreads.h>
#include <base/Types.h>


namespace Threads {
    typedef void (*ancillary_free_cb)(void* data);
    struct ancillary
    {
        atomic_rc_t rc;

        uint32 id;
        void* data;
        ancillary_free_cb free_cb;
    };

	void AncillaryRelease(ancillary* ancillary);
	void AncillaryArrayClear(ancillary*** array);

	void AncillaryArrayInit(ancillary*** array);


    ancillary* AncillaryHold(ancillary* ancillary);
    void* AncillaryGetData(const ancillary* ancillary);

    size_t AncillaryArrayCount(ancillary** array);
    int AncillaryArrayDup(
        ancillary*** dst_arrayp,
        ancillary** const* src_arrayp);

} // namespace Threads
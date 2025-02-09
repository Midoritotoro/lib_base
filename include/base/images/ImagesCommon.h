#pragma once

#include <vector>

#define RAVEL_AXIS 32
#define MAXARGS 32

namespace base::images {
    //}
    //
    //enum ORDER {
    //    /* Fortran order if inputs are all Fortran, C otherwise */
    //    NPY_ANYORDER = -1,
    //    /* C order */
    //    NPY_CORDER = 0,
    //    /* Fortran order */
    //    NPY_FORTRANORDER = 1,
    //    /* An order as close to the inputs as possible */
    //    NPY_KEEPORDER = 2
    //};

    //struct Dimensions {
    //    size_t* ptr;
    //    int len;
    //};

    //std::vector<std::vector<int>>
    //    ArrayRavel(
    //        std::vector<std::vector<int>>& arr,
    //        ORDER order)
    //{
    //    Dimensions newdim = { NULL,1 };
    //    size_t val[1] = { -1 };

    //    newdim.ptr = val;

    //    if (order == NPY_KEEPORDER) {
    //        /* This handles some corner cases, such as 0-d arrays as well */
    //        if (PyArray_IS_C_CONTIGUOUS(arr)) {
    //            order = NPY_CORDER;
    //        }
    //        else if (PyArray_IS_F_CONTIGUOUS(arr)) {
    //            order = NPY_FORTRANORDER;
    //        }
    //    }
    //    else if (order == NPY_ANYORDER) {
    //        order = PyArray_ISFORTRAN(arr) ? NPY_FORTRANORDER : NPY_CORDER;
    //    }

    //    if (order == NPY_CORDER && PyArray_IS_C_CONTIGUOUS(arr)) {
    //        return PyArray_Newshape(arr, &newdim, NPY_CORDER);
    //    }
    //    else if (order == NPY_FORTRANORDER && PyArray_IS_F_CONTIGUOUS(arr)) {
    //        return PyArray_Newshape(arr, &newdim, NPY_FORTRANORDER);
    //    }
    //    /* For KEEPORDER, check if we can make a flattened view */
    //    else if (order == NPY_KEEPORDER) {
    //        npy_stride_sort_item strideperm[NPY_MAXDIMS];
    //        npy_intp stride;
    //        int i, ndim = PyArray_NDIM(arr);

    //        PyArray_CreateSortedStridePerm(PyArray_NDIM(arr),
    //            PyArray_STRIDES(arr), strideperm);

    //        /* The output array must be contiguous, so the first stride is fixed */
    //        stride = PyArray_ITEMSIZE(arr);

    //        for (i = ndim - 1; i >= 0; --i) {
    //            if (PyArray_DIM(arr, strideperm[i].perm) == 1) {
    //                /* A size one dimension does not matter */
    //                continue;
    //            }
    //            if (strideperm[i].stride != stride) {
    //                break;
    //            }
    //            stride *= PyArray_DIM(arr, strideperm[i].perm);
    //        }

    //        /* If all the strides matched a contiguous layout, return a view */
    //        if (i < 0) {
    //            stride = PyArray_ITEMSIZE(arr);
    //            val[0] = PyArray_SIZE(arr);

    //            Py_INCREF(PyArray_DESCR(arr));
    //            return PyArray_NewFromDescrAndBase(
    //                Py_TYPE(arr), PyArray_DESCR(arr),
    //                1, val, &stride, PyArray_BYTES(arr),
    //                PyArray_FLAGS(arr), (PyObject*)arr, (PyObject*)arr);
    //        }
    //    }

    //    return PyArray_Flatten(arr, order);
    //}

    //std::vector<std::vector<int>>
    //    PyArray_CheckAxis(
    //        std::vector<std::vector<int>>& arr,
    //        int* axis,
    //        int flags)
    //{
    //    std::vector<std::vector<int>> temp1, temp2;
    //    int n = integralImageDimensions(arr);

    //    if (*axis == RAVEL_AXIS || n == 0) {
    //        if (n != 1) {
    //            temp1 = PyArray_Ravel(arr, 0);
    //            if (temp1.size() <= 0) {
    //                *axis = 0;
    //                return {};
    //            }
    //            if (*axis == RAVEL_AXIS) {
    //                *axis = integralImageDimensions(temp1) - 1;
    //            }
    //        }
    //        else {
    //            temp1 = arr;
    //            *axis = 0;
    //        }
    //        if (!flags && *axis == 0) {
    //            return temp1;
    //        }
    //    }
    //    else {
    //        temp1 = arr;
    //    }
    //    if (flags) {
    //        temp2 = PyArray_CheckFromAny(temp1, NULL,
    //            0, 0, flags, NULL);
    //        if (temp2.size() <= 0) {
    //            return {};
    //        }
    //    }
    //    else {
    //        temp2 = temp1;
    //    }
    //    n = integralImageDimensions(temp2);
    //    if (check_and_adjust_axis(axis, n) < 0) {
    //        return {};
    //    }
    //    return temp2;
    //}

    //std::vector<std::vector<int>> CumSum(
    //    const std::vector<std::vector<int>>& self,
    //    int axis)
    //{
    //    std::vector<std::vector<int>> arr, ret;

    //    arr = CheckAxis(self, &axis, 0);
    //    if (arr == NULL) {
    //        return NULL;
    //    }
    //    ret = GenericAccumulate(arr,
    //        n_ops.add, axis,
    //        rtype, out);
    //    Py_DECREF(arr);
    //    return ret;
    //}
} // namespace base::images